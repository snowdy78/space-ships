#pragma once

#include <variant>
#include "coop/Transferable.hpp"


struct incorrect_json_format : public std::exception
{
	using std::exception::exception;
};

class ClientSocket : public sf::UdpSocket
{
	sf::IpAddress ip;
	uint16_t port;

	static std::string encrypt(const std::string &str);
	static std::string decrypt(const std::string &str);

	sf::Socket::Status _send(const rn::Json &send_data);
	std::variant<rn::Json, sf::Socket::Status> recieveJson();

	using UdpSocket::receive;
	using UdpSocket::send;
	class ReceiveType
	{
		rn::Json data_json;
		ReceiveType(const rn::Json &data_json = {});
        friend class ClientSocket;    
    public:
		const Transferable::Type &type() const;
        Transferable *object() const;
        TransferableAction *action() const;
	};

public:
	ClientSocket(sf::IpAddress remote_ip, uint16_t remote_port = sf::Socket::AnyPort);
	sf::Socket::Status send(const Transferable *data);
	sf::Socket::Status
	sendAction(std::optional<size_t> author_id, std::optional<size_t> target_id, TransferableAction *action);
	std::variant<sf::Socket::Status, ReceiveType> recieve();
};
