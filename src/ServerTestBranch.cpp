#include "ServerTestBranch.hpp"


size_t Character::id = identify<Character>();

void ServerTestBranch::start()
{
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	rn::Table table{
		5,
		10,
		{ res.x / 5, res.y / 10 }
	};
	send_button.setSize(table.getCellSize(0, 0));
	send_button.setPosition(table.getCellGlobalPos(1, 1));
	send_status.setPosition(table.getCellGlobalPos(1, 2));
	receive_status.setPosition(table.getCellGlobalPos(1, 3));
	p1.setSize(table.getCellSize(0, 0));
	p2.setSize(table.getCellSize(0, 0));
	p1.setPosition(table.getCellGlobalPos(0, 0));
	p2.setPosition(table.getCellGlobalPos(1, 0));
	character.setName("meow");
	character.setRadius(25);
}
void ServerTestBranch::update()
{
	if (client)
	{
		auto various_data = client->recieve();

		if (std::holds_alternative<ClientSocket::ReceiveType>(various_data))
		{
			auto received = std::get<ClientSocket::ReceiveType>(various_data);
			if (received.is_object())
			{
				character.receiveJson(received.json()["data"]);
			}
			// TODO: with action
		}
	}
	window.clear();
	window.draw(send_button);
	window.draw(send_status);
	window.draw(receive_status);
	window.draw(p1);
	window.draw(p2);
	window.draw(character);
	window.display();
}
void ServerTestBranch::onEvent(sf::Event &event)
{
	if (event.type == sf::Event::Closed)
	{
		window.close();
	}
	if (p1.isClicked(sf::Mouse::Left))
	{
		client.reset();
		client.emplace(sf::IpAddress::getLocalAddress(), 12345);
		client->bind(25565, sf::IpAddress::getLocalAddress());
		client->setBlocking(false);
	}
	else if (p2.isClicked(sf::Mouse::Left))
	{
		client.reset();
		client.emplace(sf::IpAddress::getLocalAddress(), 25565);
		client->bind(12345, sf::IpAddress::getLocalAddress());
		client->setBlocking(false);
	}
	if (client && event.type == sf::Event::MouseMoved)
	{
		rn::Vec2f mpos{rn::mouse_position};
		character.setPosition(mpos);
		auto status = client->sendObject(&character);
		if (status == sf::Socket::Done)
		{
			send_status.setString("Successfully sended!");
			std::cout << "data sended:" << character.toJson().dump() << "\n";
		}
		else
		{
			std::cout << "failed to send\n";
			send_status.setString("Failed to send! code: " + std::to_string(status));
		}
	}
}
