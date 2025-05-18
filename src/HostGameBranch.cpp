#include "HostGameBranch.hpp"
#include <memory>
#include "MainMenu.hpp"
#include "coop/Request.hpp"
#include "coop/UdpRouter.hpp"
#include "game/GameManager.hpp"
#include "game/GameObjectFactory.hpp"
#include "game/SpaceField.hpp"
#include "game/GameObjectTranslator.hpp"

HostGameBranch::~HostGameBranch()
{
	window.setView(window.getDefaultView());
	GameManager::clear();
}

void HostGameBranch::start()
{
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	GameManager::client(window, [this] {
		background.setPosition(space->camera.getPosition());
		},
		{ ip_address, port }
	);
	rn::Table table{
		5,
		10,
		{ res.x / 5, res.y / 10 }
	};
	send_status.setPosition(table.getCellGlobalPos(1, 2));
	receive_status.setPosition(table.getCellGlobalPos(1, 3));
	if (GameManager::exist())
		space = &GameManager::instance();
	space->action_manager.setTransfering(TransferType::Tcp);
	online	= space->online.get();
	auto ls = online->tcp->host();
	if (ls == sf::Socket::Done)
		std::cout << "listening\n";
	online->tcp->setBlocking(false);
	if (space)
	{
		space->player->setPosition(res / 2.f);
		background.start();
		space->field.start();
		space->action_manager.start();
	}
}
void HostGameBranch::update()
{
	if (!space || !online || !window.isOpen())
		return;
	auto connection_status = online->tcp->findConnection();
	if (connection_status == sf::Socket::Disconnected)
	{
		std::cout << "disconnection\n";
		next_branch<MainMenu>(window);
		return;
	}
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
void HostGameBranch::onEvent(sf::Event &event)
{
	if (!window.hasFocus())
		return;
	if (event.type == sf::Event::Closed)
		window.close();
	if (rn::isKeydown(sf::Keyboard::Escape))
		next_branch<MainMenu>(window);
	space->field.onEvent(event);
	space->action_manager.onEvent(event);
}

void HostGameBranch::receivePackets() const
{
	auto response = online->tcp->receive();
	if (response.success())
	{
		if (response.is_object())
		{
			auto object = response.object();
			if (auto request_ptr = dynamic_cast<Request *>(object.get()))
			{
				auto &request = *request_ptr;
				std::cout << "player connected!\n";
				if (request.contains("type") && request["type"] == "connect")
				{
					std::cout << "sending game data...\n";
					GameObjectTranslator translator;
					translator.assignUpdateData(
						GameObjectFactory::instance().begin(), GameObjectFactory::instance().end()
					);
					auto status = online->tcp->send(&translator);
					if (status != sf::Socket::Done)
						std::cerr << "Failed to send with code: " << status << "\n";
					else
						std::cout << "Successfully sent data: " << translator.toJson().dump(2, ' ', '\n') << "\n";
				}
			}
			if (auto space_object = dynamic_cast<SpaceFieldObject *>(object.get()))
				space_object->summonCopy(space->field);
		}
		if (response.is_action())
		{
			space->action_manager.receiveToTop(response.action());
		}
	}
}
