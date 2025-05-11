#pragma once

#include "SFML/Network/IpAddress.hpp"
#include "decl.hpp"
#include "Router.hpp"

class TcpRouter : public sf::TcpSocket, public Router
{
	static std::string encrypt(const std::string &str);
	static std::string decrypt(const std::string &str);

	sf::Socket::Status sendJson(const rn::Json &send_data) override;
	StatusOrJson receiveJson() override;

	using sf::TcpSocket::receive;
	using sf::TcpSocket::send;
	using sf::TcpSocket::setBlocking;
public:
	using Router::receive;
	using Router::send;

	TcpRouter(sf::IpAddress host_ip, uint16_t host_port = sf::Socket::AnyPort);
	void setBlocking(bool blocking);

	Status findConnection();
	Status host();
	sf::Socket::Status connect(const sf::IpAddress &remote_ip, const uint16_t &remote_port);
private:

	sf::IpAddress m_host_ip;
	uint16_t m_host_port;
	std::optional<uint16_t> m_self_port;
	std::optional<sf::IpAddress> m_self_ip;
	sf::TcpListener m_listener;
};
