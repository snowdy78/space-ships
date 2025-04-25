#include "HostGameBranch.hpp"
#include <memory>
#include "coop/Request.hpp"
#include "coop/TransferableAction.hpp"
#include "coop/UdpRouter.hpp"
#include "game/GameObjectFabric.hpp"
#include "game/SpaceField.hpp"


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
	
	space = GameGlobals::instance();
	client.setBlocking(false);
	client.bind(remote_port, ip_address);
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
void HostGameBranch::onEvent(sf::Event &event)
{
	if (event.type == sf::Event::Closed)
	{
		window.close();
	}
	space->field.onEvent(event);
	space->action_manager.onEvent(event);
}

void HostGameBranch::receivePackets()
{
	auto various_data = client.recieve();
	if (std::holds_alternative<UdpRouter::Response>(various_data))
	{
		auto received = std::get<UdpRouter::Response>(various_data);
		if (received.is_object())
		{
			auto object = received.object();
			if (auto request_ptr = dynamic_cast<Request *>(object.get()))
			{
				auto &request = *request_ptr;
				if (request.contains("type") && request["type"] == "connect")
				{
					GameObjectFabricTranslator translator;
					translator.assignUpdateData(GameObjectFabric::instance().begin(), GameObjectFabric::instance().end());
					auto status = client.sendObject(&translator);
					if (status != sf::Socket::Done)
					{
						std::cerr << "Failed to send with code: " << status << "\n";
					}
					else 
					{
						std::cout << "Successfully sent data: " << translator.toJson().dump(2, ' ', '\n') << "\n";
					}
				}
			}
			if (auto space_object = dynamic_cast<SpaceFieldObject *>(object.get()))
			{
				space_object->summonCopy(&space->field);
			}
		}
		if (received.is_action())
		{
			std::unique_ptr<AbstractAction> action{ received.action().release() };
			if (action)
			{
				space->action_manager.addToTop(std::move(action));
			}
		}
	}
}
