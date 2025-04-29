#pragma once

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
	sf::TcpListener listener;
public:
	using Router::receive;
	using Router::send;

	TcpRouter(sf::IpAddress remote_ip, uint16_t remote_port = sf::Socket::AnyPort);

	void setBlocking(bool blocking);
	sf::Socket::Status connect();
};
