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
    std::cout << "my ip is " << sf::IpAddress::getLocalAddress() << "\n";
	std::cout << "type remote address 'ip:port': ";
	std::string address;
	std::cin >> address;
	std::regex ip_port_rgx(R"(([0-9]*\.[0-9]*\.[0-9]*\.[0-9]*|local|l):([0-9]{5}))");
	std::smatch matches;
	if (std::regex_search(address, matches, ip_port_rgx))
	{
		if (matches.size() == 3)
		{
			sf::IpAddress ip_address = std::regex_match(address, std::regex("l|local"))
										   ? sf::IpAddress::getLocalAddress()
										   : sf::IpAddress(matches[1]);
			uint16_t port			 = std::stoi(matches[2]);
			client.emplace(ip_address, port);
		}
	}
	if (client)
	{
		Character p(1, "test");
		if (client->send(&p))
		{
			std::cout << "sent\n";
		}
		else
		{
			std::cout << "not sent\n";
		}
	}
}
void ServerTestBranch::update() {}
void ServerTestBranch::onEvent(sf::Event &event) {}
