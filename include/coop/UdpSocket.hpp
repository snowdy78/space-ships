#pragma once

#include "decl.hpp"
#include <variant>
#include "Transferable.hpp"

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

class UdpSocket : public sf::UdpSocket
{
	struct TransferType
	{
		inline static constexpr std::string object = "object";
		inline static constexpr std::string action = "action";
	};
	sf::IpAddress ip;
	uint16_t port;

	static std::string encrypt(const std::string &str);
	static std::string decrypt(const std::string &str);

	sf::Socket::Status _send(const rn::Json &send_data);
	std::variant<rn::Json, sf::Socket::Status> recieveJson();

	using sf::UdpSocket::receive;
	using sf::UdpSocket::send;
	
public:
	class ReceiveType
	{
		Transferable::TransferJson transfer_json;
		ReceiveType(const rn::Json &data_json = {});
        friend class UdpSocket;    
    public:
		bool is_unknown() const;
		bool is_action() const;
		bool is_object() const;
        std::unique_ptr<TransferableObject> object() const;
        std::unique_ptr<TransferableAction> action() const;
		const Transferable::TransferJson &json() const;
	};
	UdpSocket(sf::IpAddress remote_ip, uint16_t remote_port = sf::Socket::AnyPort);
	sf::Socket::Status sendObject(const TransferableObject *data);
	sf::Socket::Status
	sendAction(std::optional<size_t> author_id, std::optional<size_t> target_id, TransferableAction *action);
	std::variant<sf::Socket::Status, ReceiveType> recieve();
};
