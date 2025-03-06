#include "ServerTestBranch.hpp"
#include "Font.hpp"
#include "SFML/Network/IpAddress.hpp"

class Character : public TransferObject
{
	size_t id;
	std::string name;

public:
	Character(size_t id, std::string name)
		: id(id),
		  name(name)
	{}
	TransferInstance toJson() const override
	{
		return {
			{ "id",	id   },
			{ "name", name }
		};
	}
};


void ServerTestBranch::start()
{
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	rn::Table table{5, 10, {res.x/5, res.y/10}};
	send_button.setSize(table.getCellSize(0, 0));
	send_button.setPosition(table.getCellGlobalPos(1, 1));
	send_status.setPosition(table.getCellGlobalPos(1, 2));
	receive_status.setPosition(table.getCellGlobalPos(1, 3));
	p1.setSize(table.getCellSize(0, 0));
	p2.setSize(table.getCellSize(0, 0));
	p1.setPosition(table.getCellGlobalPos(0, 0));
	p2.setPosition(table.getCellGlobalPos(1, 0));
}
void ServerTestBranch::update() 
{
	if (client)
	{
		if (auto data = client->recieve())
        {
			if (data->contains("id"))
				std::cout << "containing an id\n";
			receive_status.setString("got: " + data->dump() + "\n");
        }
	}
	window.clear();
	window.draw(send_button);
	window.draw(send_status);
	window.draw(receive_status);
	window.draw(p1);
	window.draw(p2);
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
	if (client && send_button.isClicked(sf::Mouse::Left))
	{
		Character p(1, "test");
		auto status = client->send(&p);
		if (status == sf::Socket::Done)
		{
			send_status.setString("Successfully sended!");
			std::cout << "sended\n";
		}
		else
		{
			std::cout << "failed to send\n";
			send_status.setString("Failed to send! code: " + std::to_string(status));
		}
	}
}

