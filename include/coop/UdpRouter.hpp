#pragma once

#include "decl.hpp"
#include <variant>
#include "Router.hpp"

struct incorrect_json_format : public std::exception
{
	using std::exception::exception;
	incorrect_json_format(const std::string &msg) : message(msg) {}
	char const *what() const noexcept override
	{
		return message.c_str();
	}
private:
	std::string message;
};
class UdpRouter : public sf::UdpSocket, public Router
{	
	static std::string encrypt(const std::string &str);
	static std::string decrypt(const std::string &str);

	sf::Socket::Status _send(const rn::Json &send_data);
	std::variant<rn::Json, sf::Socket::Status> recieveJson();

	using sf::UdpSocket::receive;
	using sf::UdpSocket::send;
	
public:
	UdpRouter(sf::IpAddress remote_ip, uint16_t remote_port = sf::Socket::AnyPort);
	sf::Socket::Status sendObject(const TransferableObject *data) override;
	sf::Socket::Status
	sendAction(std::optional<size_t> author_id, std::optional<size_t> target_id, TransferableAction *action) override;
	std::variant<sf::Socket::Status, Router::Response> recieve() override;
private:
	sf::IpAddress ip;
	uint16_t port;
};
