#include "coop/TcpRouter.hpp"
#include "SFML/Network/IpAddress.hpp"

TcpRouter::TcpRouter(sf::IpAddress remote_ip, uint16_t remote_port)
	: TcpSocket(), m_host_ip(remote_ip), m_host_port(remote_port)
{
}

void TcpRouter::setBlocking(bool blocking)
{
	m_listener.setBlocking(blocking);
	sf::TcpSocket::setBlocking(blocking);
}

sf::Socket::Status TcpRouter::findConnection()
{
	return m_listener.accept(*this);
}

sf::Socket::Status TcpRouter::connect(const sf::IpAddress &remote_ip, const uint16_t &remote_port)
{
	return sf::TcpSocket::connect(remote_ip, remote_port);
}

sf::Socket::Status TcpRouter::host() 
{
	return m_listener.listen(m_host_port, m_host_ip);
}

std::string TcpRouter::encrypt(const std::string &str) 
{
	return str;
}

std::string TcpRouter::decrypt(const std::string &str) 
{
	return str;
}

sf::Socket::Status TcpRouter::sendJson(const rn::Json &send_data) 
{
	std::string json_string = encrypt(send_data.dump(-1, 0));
	sf::Packet packet;
	packet.append(json_string.c_str(), (json_string.size() + 1) * sizeof(char));
	Status status = TcpRouter::send(packet);
	std::cout << "sended packet: " << json_string << "\n";
	return status;
}

Router::StatusOrJson TcpRouter::receiveJson()
{
	sf::Packet packet;
	Status receive_status = receive(packet);
	if (receive_status != sf::Socket::Done)
		return receive_status;
	std::string str{ static_cast<const char *>(packet.getData()) };
	rn::Json json = rn::Json::parse(decrypt(str));
	std::cout << "received packet: " << str << "\n"; 
	return json;
}
