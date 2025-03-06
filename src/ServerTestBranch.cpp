#include "ServerTestBranch.hpp"
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
	std::cout << rn::Json::parse("{ \"id\": 1, \"name\": \"test\" }").dump(-1, 0) << "\n";
    std::cout << "my ip is " << sf::IpAddress::getLocalAddress() << "\n";
	std::cout << "type remote address 'ip:port': ";
	std::string address;
	std::cin >> address;
	std::regex ip_port_rgx(R"(([0-9]*\.[0-9]*\.[0-9]*\.[0-9]*|local|l)(:)?([0-9]{0,5}))");
	std::smatch matches;
	if (std::regex_search(address, matches, ip_port_rgx))
	{
		if (matches.size() >= 2)
		{
			sf::IpAddress ip_address = std::regex_match(address, std::regex("l|local"))
										   ? sf::IpAddress::getLocalAddress()
										   : sf::IpAddress(matches[1]);
			uint16_t port = sf::Socket::AnyPort;
			if (matches.size() == 3) 
			{
				port = std::stoi(matches[2]);
			}
			client.emplace(ip_address, port);
		}
	}
	else 
	{
		std::cout << "client is not initialized\n";
	}
	if (client) 
	{
		client->setBlocking(false);
	}
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	rn::Table table{5, 10, {res.x/5, res.y/10}};
	send_button.setSize(table.getCellSize(0, 0));
	send_button.setPosition(table.getCellGlobalPos(1, 1));
	send_status.setPosition(table.getCellGlobalPos(1, 2));
	receive_status.setPosition(table.getCellGlobalPos(1, 3));
}
void ServerTestBranch::update() 
{
	if (client)
	{
		if (auto data = client->recieve())
        {
			receive_status.setString("got: " + data->dump(4, ' ', "\n") + "\n");
        }
	}
	window.clear();
	window.draw(send_button);
	window.draw(send_status);
	window.draw(receive_status);
	window.display();
}
void ServerTestBranch::onEvent(sf::Event &event) 
{
	if (event.type == sf::Event::Closed)
	{
		window.close();
	}
	if (send_button.isClicked(sf::Mouse::Left))
	{
		Character p(1, "test");
		auto status = client->send(&p);
		if (status == sf::Socket::Done)
		{
			send_status.setString("Successfully sended!");
		}
		else
		{
			send_status.setString("Failed to send! code: " + std::to_string(status));
		}
	}
}

