#include "ConnectToGameBranch.hpp"
#include <memory>
#include "MainMenu.hpp"
#include "RuneEngine/EngineDecl.hpp"
#include "coop/Request.hpp"
#include "coop/UdpRouter.hpp"
#include "game/GameGlobals.hpp"
#include "game/GameObjectFabric.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/AbstractAction.hpp"

class TestAction : public TransferableAction
{
public:
	TestAction(GameObject *author = nullptr, GameObject *contributor = nullptr, const rn::Json &props = nullptr)
		: TransferableAction(author, contributor, props)
	{
	}
	void play() override
	{
		std::cout << "action played!";
	}
	TransferJson toJson() const override
	{
		return { id };
	}
	AbstractAction *copy() const override
	{
		return new TestAction();
	}
private:
	static const size_t id;
};
const size_t TestAction::id = identify<TestAction>();


ConnectToGameBranch::~ConnectToGameBranch()
{
	window.setView(window.getDefaultView());
	GameGlobals::clear();
}

void ConnectToGameBranch::start()
{
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	GameGlobals::create(window, [this] {
		background.setPosition(space->camera.getPosition());
	});
	if (GameGlobals::exist())
		space = &GameGlobals::instance();
	rn::Table table{
		5,
		10,
		{ res.x / 5, res.y / 10 }
	};
	space->createOnline({host_ip, host_port});
	online	= space->online.get();
	auto cs = online->tcp->connect(host_ip, host_port);
	if (cs == sf::Socket::Done)
	{
		std::cout << "connected\n";
		Request request;
		request["type"] = "connect";
		auto status		= online->tcp->send(&request);
		if (status != sf::Socket::Done)
			std::cerr << "Failed to send with code: " << status << "\n";
		else
			std::cout << "Successfully sent data: " << request.toJson().dump(2, ' ', '\n') << "\n";
	}

	send_status.setPosition(table.getCellGlobalPos(1, 2));
	receive_status.setPosition(table.getCellGlobalPos(1, 3));
	online->tcp->setBlocking(false);
	if (space)
	{
		space->player->setPosition(res / 2.f);
		background.start();
		space->field.start();
		space->action_manager.start();
	}
}
void ConnectToGameBranch::update()
{
	if (!space || !window.isOpen())
		return;

	space->field.update();
	space->action_manager.update();
	background.update();
	receivePackets();
	window.clear();
	sf::Transform bg_transform = space->camera.getTransform();
	window.draw(background, bg_transform);
	window.draw(space->field);
	window.draw(send_status);
	window.draw(receive_status);
	window.display();
}
void ConnectToGameBranch::onEvent(sf::Event &event)
{
	if (!window.hasFocus())
		return;
	if (event.type == sf::Event::Closed)
		window.close();
	if (rn::isKeydown(sf::Keyboard::Escape))
		next_branch<MainMenu>(window);
	space->field.onEvent(event);
	space->action_manager.onEvent(event);
	if (rn::isKeydown(sf::Keyboard::N))
	{
		std::cout << "sending an action...\n";
		TestAction action(nullptr, nullptr, {});
		if (auto status = online->tcp->send(&action); status == sf::Socket::Status::Done)
		{
			std::cout << "action was successfully send\n";
		}
	}
}

void ConnectToGameBranch::receivePackets() const
{
	if (!space)
		return;
	auto response = online->tcp->receive();
	if (response.success())
	{
		if (response.is_object())
		{
			auto object = response.object();
			if (auto translator = dynamic_cast<GameObjectFabricTranslator *>(object.get()))
			{
				if (translator->getTranslateType() == GameObjectFabricTranslator::TranslateType::Append)
				{
					auto objects = GameObjectFabric::instance().update(*translator);
					for (auto &unique_object: objects)
						if (auto space_object = dynamic_cast<SpaceFieldObject *>(unique_object.get()))
							space_object->summonCopy(&space->field);
				}
				else if (translator->getTranslateType() == GameObjectFabricTranslator::TranslateType::Clear)
				{
					GameObjectFabric::instance().remove(*translator, [&](GameObject *game_object) {
						if (auto space_object = dynamic_cast<SpaceFieldObject *>(game_object))
							space_object->destroy();
					});
				}
			}
			if (auto space_object = dynamic_cast<SpaceFieldObject *>(object.get()))
				space_object->summonCopy(&space->field);
		}
		if (response.is_action())
		{
			if (std::unique_ptr<AbstractAction> action{ response.action().release() })
				space->action_manager.addToTop(std::move(action));
		}
	}
}
