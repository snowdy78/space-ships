#pragma once

#include "decl.hpp"
#include "Router.hpp"

class UdpRouter : public sf::UdpSocket, public Router
{	
	static std::string encrypt(const std::string &str);
	static std::string decrypt(const std::string &str);

	sf::Socket::Status sendJson(const rn::Json &send_data) override;
	StatusOrJson receiveJson() override;

	using sf::UdpSocket::receive;
	using sf::UdpSocket::send;
	
public:
	UdpRouter(sf::IpAddress remote_ip, uint16_t remote_port = sf::Socket::AnyPort);
	using Router::receive;
	using Router::send;
private:
	sf::IpAddress ip;
	uint16_t port;
};
