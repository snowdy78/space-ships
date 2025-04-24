#include "ConnectToGameBranch.hpp"
#include <memory>
#include "coop/TransferableAction.hpp"
#include "coop/UdpSocket.hpp"
#include "game/EnemyShip.hpp"
#include "game/GameObjectFabric.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/AbstractAction.hpp"


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
	space = GameGlobals::instance();
	client.setBlocking(false);
	client.bind(remote_port, ip_address);
	if (space)
	{
		space->player->setPosition(res / 2.f);
		space->field.start();
		space->field.appendShip<EnemyShip>();
	}
}
void ConnectToGameBranch::update()
{
	if (!space)
		return;
	space->field.update();
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
	space->field.onEvent(event);
}

void ConnectToGameBranch::receivePackets()
{
	if (!space)
		return;
	auto various_data = client.recieve();
	if (std::holds_alternative<UdpSocket::ReceiveType>(various_data))
	{
		auto received = std::get<UdpSocket::ReceiveType>(various_data);
		if (received.is_object())
		{
			auto object = received.object();
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
		if (received.is_action())
		{
			std::unique_ptr<AbstractAction> action = std::move(received.action());
			if (action)
			{
				space->action_manager.addToTop(std::move(action));
			}
		}
	}
}
