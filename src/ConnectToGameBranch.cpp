#include "ConnectToGameBranch.hpp"
#include <memory>
#include "MainMenu.hpp"
#include "RuneEngine/EngineDecl.hpp"
#include "coop/Request.hpp"
#include "game/GameManager.hpp"
#include "game/GameObjectTranslator.hpp"
#include "game/SpaceField.hpp"

ConnectToGameBranch::~ConnectToGameBranch()
{
	window.setView(window.getDefaultView());
	GameManager::clear();
}

void ConnectToGameBranch::start()
{
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	GameManager::host(
		window,
		{ host_ip, host_port }
	);
	if (GameManager::exist())
		session = GameManager::session();
	rn::Table table{
		5,
		10,
		{ res.x / 5, res.y / 10 }
	};
	online	= GameManager::instance().online.get();
	auto cs = online->tcp->connect(host_ip, host_port);
	if (cs == sf::Socket::Done)
	{
#ifdef SPACE_SHIP_DEBUG
		std::cout << "connected\n";
#endif
		Request request;
		request["type"] = "connect";
		auto status		= online->tcp->send(&request);
#ifdef SPACE_SHIP_DEBUG
		if (status != sf::Socket::Done)
			std::cerr << "Failed to send with code: " << status << "\n";
		else
			std::cout << "Successfully sent data: " << request.requestData().dump(2, ' ', '\n') << "\n";
#endif
	}
	GameManager::session()->action_manager.setTransfering(TransferType::Tcp);
	send_status.setPosition(table.getCellGlobalPos(1, 2));
	receive_status.setPosition(table.getCellGlobalPos(1, 3));
	online->tcp->setBlocking(false);
	if (session)
	{
		session->start();
		background.start();
		GameManager::session()->action_manager.start();
	}
}
void ConnectToGameBranch::update()
{
	if (!session || !window.isOpen())
		return;

	session->update();
	GameManager::session()->action_manager.update();
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
void ConnectToGameBranch::onEvent(sf::Event &event)
{
	if (!window.hasFocus())
		return;
	if (event.type == sf::Event::Closed)
		window.close();
	if (rn::isKeydown(sf::Keyboard::Escape))
		next_branch<MainMenu>(window);
	session->onEvent(event);
	GameManager::session()->action_manager.onEvent(event);
}

void ConnectToGameBranch::receivePackets() const
{
	if (!session)
		return;
	auto response = online->tcp->receive();
	if (response.success())
	{
		if (response.is_object())
		{
			auto object = response.object();
			if (auto translator = dynamic_cast<GameObjectTranslator *>(object.get()))
			{
				if (translator->getTranslateType() == GameObjectTranslator::TranslateType::Append)
				{
					GameObjectFactory::instance().update(*translator, [this](const std::unique_ptr<GameObject> &object) {
#ifdef SPACE_SHIP_DEBUG
						std::cout << "appending a new object on space field...\n";
#endif
						if (auto space_object = dynamic_cast<SpaceFieldObject *>(object.get()))
						{
#ifdef SPACE_SHIP_DEBUG
							std::cout << "summoning...\n";
#endif
							space_object->summonCopy(session->field);
#ifdef SPACE_SHIP_DEBUG
							std::cout << "summoned!\n";
#endif
						}
					});
				}
				else if (translator->getTranslateType() == GameObjectTranslator::TranslateType::Clear)
				{
					GameObjectFactory::instance().remove(*translator, [&](GameObject *game_object) {
						if (auto space_object = dynamic_cast<SpaceFieldObject *>(game_object))
							space_object->destroy();
					});
				}
			}
			if (auto space_object = dynamic_cast<SpaceFieldObject *>(object.get()))
			{
#ifdef SPACE_SHIP_DEBUG
				std::cout << "received space field object\n";
#endif
				space_object->summonCopy(session->field);
#ifdef SPACE_SHIP_DEBUG
				std::cout << "summoned object\n";
#endif
			}
		}
		if (response.is_action())
		{
			auto action = response.action(GameManager::session()->field);
			GameManager::session()->action_manager.receiveToTop(std::move(action));
		}
	}
}
