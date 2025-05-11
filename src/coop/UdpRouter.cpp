#include "coop\UdpRouter.hpp"

UdpRouter::UdpRouter() = default;

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
	Status status = UdpRouter::send(packet, m_ip, m_port);
	return status;
}

Router::StatusOrJson UdpRouter::receiveJson()
{
	sf::Packet packet;
	sf::IpAddress ip2 = m_ip;
	auto port2		  = m_port;
	Status status	  = sf::UdpSocket::receive(packet, ip2, port2);
	if (status != sf::Socket::Done)
		return status;
	std::string str{ static_cast<const char *>(packet.getData()) };
	rn::Json json = rn::Json::parse(decrypt(str));
	return json;
}

void UdpRouter::bindRemote(sf::IpAddress remote_ip, uint16_t remote_port)
{
	m_ip = remote_ip;
	m_port = remote_port;
}

const sf::IpAddress & UdpRouter::getRemoteIp() const
{
	return m_ip;
}

const uint16_t & UdpRouter::getRemotePort() const
{
	return m_port;
}
