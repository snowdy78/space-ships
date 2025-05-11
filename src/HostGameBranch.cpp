#include "HostGameBranch.hpp"
#include <memory>
#include "MainMenu.hpp"
#include "coop/Request.hpp"
#include "coop/UdpRouter.hpp"
#include "game/GameGlobals.hpp"
#include "game/GameObjectFabric.hpp"
#include "game/SpaceField.hpp"

HostGameBranch::~HostGameBranch()
{
	window.setView(window.getDefaultView());
	GameGlobals::clear();
}

void HostGameBranch::start()
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
	space->createOnline({ ip_address, port });
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
	space->field.onEvent(event);
	space->action_manager.onEvent(event);
}

void HostGameBranch::receivePackets() const
{
	auto response = online->tcp->receive();
	if (response.success())
	{
		std::cout << "response is success and status " << response.status() << " and " << response.is_object()
				  << " and " << response.is_action() << "\n";
		if (response.is_object())
		{
			auto object = response.object();
			if (auto request_ptr = dynamic_cast<Request *>(object.get()))
			{
				auto &request = *request_ptr;
				if (request.contains("type") && request["type"] == "connect")
				{
					std::cout << "sending game data...\n";
					GameObjectFabricTranslator translator;
					translator.assignUpdateData(
						GameObjectFabric::instance().begin(), GameObjectFabric::instance().end()
					);
					auto status = online->tcp->send(&translator);
					if (status != sf::Socket::Done)
						std::cerr << "Failed to send with code: " << status << "\n";
					else
						std::cout << "Successfully sent data: " << translator.toJson().dump(2, ' ', '\n') << "\n";
				}
			}
			if (auto space_object = dynamic_cast<SpaceFieldObject *>(object.get()))
				space_object->summonCopy(&space->field);
		}
		if (response.is_action())
		{
			space->action_manager.receiveToTop(response.action());
		}
	}
}
