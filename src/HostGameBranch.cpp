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
		background.setPosition(session->camera.getPosition());
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
		session = GameManager::session();
	GameManager::instance().action_manager.setTransfering(TransferType::Tcp);
	online	= GameManager::instance().online.get();
	auto ls = online->tcp->host();
	if (ls == sf::Socket::Done)
		std::cout << "listening\n";
	online->tcp->setBlocking(false);
	if (session)
	{
		session->player->setPosition(res / 2.f);
		background.start();
		session->field.start();
		GameManager::instance().action_manager.start();
	}
}
void HostGameBranch::update()
{
	if (!session || !online || !window.isOpen())
		return;
	auto connection_status = online->tcp->findConnection();
	if (connection_status == sf::Socket::Disconnected)
	{
		std::cout << "disconnection\n";
		next_branch<MainMenu>(window);
		return;
	}
	session->field.update();
	GameManager::instance().action_manager.update();
	background.update();
	receivePackets();
	window.clear();
	sf::Transform bg_transform = session->camera.getTransform();
	window.draw(background, bg_transform);
	window.draw(session->field);
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
	session->field.onEvent(event);
	GameManager::instance().action_manager.onEvent(event);
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
						std::cout << "Successfully sent data: " << translator.requestData().dump(2, ' ', '\n') << "\n";
				}
			}
			if (auto space_object = dynamic_cast<SpaceFieldObject *>(object.get()))
				space_object->summonCopy(session->field);
		}
		if (response.is_action())
		{
			GameManager::instance().action_manager.receiveToTop(response.action());
		}
	}
}
