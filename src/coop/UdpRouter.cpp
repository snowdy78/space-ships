#include "coop\UdpRouter.hpp"

UdpRouter::UdpRouter(sf::IpAddress ip, uint16_t port)
	: sf::UdpSocket(),
	ip(ip),
	port(port)
{}

std::string UdpRouter::encrypt(const std::string &str)
{
	// TODO: encryption of data
	return str;
}

std::string UdpRouter::decrypt(const std::string &str)
{
	// TODO: decryption of data
	return str;
}

sf::Socket::Status UdpRouter::sendJson(const rn::Json &send_data)
{
	std::string json_string = encrypt(send_data.dump(-1, 0));
	sf::Packet packet;
	packet.append(json_string.c_str(), (json_string.size() + 1) * sizeof(char));
	Status status = UdpRouter::send(packet, ip, port);
	return status;
}

Router::StatusOrJson UdpRouter::receiveJson()
{
	sf::Packet packet;
	sf::IpAddress ip2 = ip;
	auto port2		  = port;
	Status status	  = sf::UdpSocket::receive(packet, ip2, port2);
	if (status != sf::Socket::Done)
		return status;
	std::string str{ static_cast<const char *>(packet.getData()) };
	rn::Json json = rn::Json::parse(decrypt(str));
	return json;
}
