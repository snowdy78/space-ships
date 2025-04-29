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


ConnectToGameBranch::~ConnectToGameBranch()
{
	window.setView(window.getDefaultView());
    if (GameGlobals::exist())
	{
		GameGlobals::instance().clear();
	}
}

void ConnectToGameBranch::start()
{
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	rn::Table table{
		5,
		10,
		{ res.x / 5, res.y / 10 }
	};

	send_status.setPosition(table.getCellGlobalPos(1, 2));
	receive_status.setPosition(table.getCellGlobalPos(1, 3));
	GameGlobals::create(window, [this] {
		background.setPosition(space->camera.getPosition());
	});
	if (GameGlobals::exist())
		space = &GameGlobals::instance();
	client.setBlocking(false);
	client.bind(remote_port, ip_address);
	if (space)
	{
		space->player->setPosition(res / 2.f);
		background.start();
		space->field.start();
		space->action_manager.start();
		Request request;
		request["type"] = "connect";
		auto status = client.send(&request);
		if (status != sf::Socket::Done)
		{
			std::cerr << "Failed to send with code: " << status << "\n";
		}
		else
		{
			std::cout << "Successfully sent data: " << request.toJson().dump(2, ' ', '\n') << "\n";
		}
	}
}
void ConnectToGameBranch::update()
{
	if (!space)
		return;
	space->field.update();
	space->action_manager.update();
	background.update();
	receivePackets();
	window.clear();
	sf::Transform bg_transform;
	bg_transform = space->camera.getTransform();
	window.draw(background, bg_transform);
	window.draw(space->field);
	window.draw(send_status);
	window.draw(receive_status);
	window.display();
}
void ConnectToGameBranch::onEvent(sf::Event &event)
{
	if (event.type == sf::Event::Closed)
	{
		window.close();
	}
	if (rn::isKeydown(sf::Keyboard::Escape))
	{
		next_branch<MainMenu>(window);
	}
	space->field.onEvent(event);
	space->action_manager.onEvent(event);
}

void ConnectToGameBranch::receivePackets()
{
	if (!space)
		return;
	auto response = client.receive();
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
					{
						if (auto space_object = dynamic_cast<SpaceFieldObject *>(unique_object.get()))
						{
							space_object->summonCopy(&space->field);
						}
					}
				}
				else if (translator->getTranslateType() == GameObjectFabricTranslator::TranslateType::Clear)
				{
					GameObjectFabric::instance().remove(*translator, [&](GameObject *game_object) {
						if (auto space_object = dynamic_cast<SpaceFieldObject *>(game_object))
						{
							space_object->destroyFromField();
						}
					});
				}
			}
			if (auto space_object = dynamic_cast<SpaceFieldObject *>(object.get()))
			{
				space_object->summonCopy(&space->field);
			}
		}
		if (response.is_action())
		{
			std::unique_ptr<AbstractAction> action = std::move(response.action());
			if (action)
			{
				space->action_manager.addToTop(std::move(action));
			}
		}
	}
}
