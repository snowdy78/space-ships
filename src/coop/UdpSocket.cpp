#include "coop\UdpSocket.hpp"
#include <stdexcept>
#include "coop/TransferableAction.hpp"
#include "coop/TransferableObject.hpp"
#include "game/GameObjectFabric.hpp"


UdpSocket::UdpSocket(sf::IpAddress ip, uint16_t port)
	: sf::UdpSocket(),
	  ip(ip),
	  port(port)
{}

std::string UdpSocket::encrypt(const std::string &str)
{
	// TODO: encryption of data
	return str;
}

std::string UdpSocket::decrypt(const std::string &str)
{
	// TODO: decryption of data
	return str;
}

sf::Socket::Status UdpSocket::_send(const rn::Json &send_data)
{
	std::string json_string = encrypt(send_data.dump(-1, 0));
	sf::Packet packet;
	packet.append(json_string.c_str(), (json_string.size() + 1) * sizeof(char));
	Status status = UdpSocket::send(packet, ip, port);
	return status;
}

std::variant<rn::Json, sf::Socket::Status> UdpSocket::recieveJson()
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

sf::Socket::Status UdpSocket::sendObject(const TransferableObject *data)
{
	auto send_data	  = data->toJson();
	send_data["type"] = TransferType::object;
	return _send(send_data);
}

sf::Socket::Status
UdpSocket::sendAction(std::optional<size_t> author_id, std::optional<size_t> target_id, TransferableAction *action)
{
	using namespace std::string_literals;
	if (!action)
		return sf::Socket::Status::Error;
	rn::Json send_data = action->toJson();
	send_data["type"]  = TransferType::action;
	if (author_id)
		send_data["author_id"] = *author_id;
	else
		send_data["author_id"] = nullptr;
	if (target_id)
		send_data["target_id"] = *target_id;
	else
		send_data["target_id"] = nullptr;
	return _send(send_data);
}

std::variant<sf::Socket::Status, UdpSocket::ReceiveType> UdpSocket::recieve()
{
	auto json_or_status = recieveJson();
	if (std::holds_alternative<sf::Socket::Status>(json_or_status))
	{
		return std::get<sf::Socket::Status>(json_or_status);
	}
	auto &json = std::get<rn::Json>(json_or_status);
	if (!json.contains("type") || !json.at("type").is_string())
		throw incorrect_json_format("invalid json format");
	auto type = json.at("type");
	if (type != TransferType::action && type != TransferType::object)
		throw incorrect_json_format("invalid type name");
	std::cout << "response body: " << json.dump(-1, 0) << "\n"; // TODO: remove
	return UdpSocket::ReceiveType(json);
}

UdpSocket::ReceiveType::ReceiveType(const rn::Json &data_json)
	: transfer_json(data_json["id"], data_json)
{}

bool UdpSocket::ReceiveType::is_unknown() const
{
	return !is_action() && !is_object();
}

bool UdpSocket::ReceiveType::is_action() const
{
	return transfer_json["type"] == TransferType::action;
}

bool UdpSocket::ReceiveType::is_object() const
{
	return transfer_json["type"] == TransferType::object;
}

std::unique_ptr<TransferableAction> UdpSocket::ReceiveType::action() const
{
	if (!is_action())
		return nullptr;
	try
	{
		auto &author = GameObjectFabric::instance().get(transfer_json["author_id"]);
		auto &target = GameObjectFabric::instance().get(transfer_json["target_id"]);
		return std::move(
			TransferableActionFabric::instance().get(transfer_json.id())(author, target, transfer_json.data())
		);
	}
	catch (rn::Json::out_of_range &err)
	{}
	return nullptr;
}

const Transferable::TransferJson &UdpSocket::ReceiveType::json() const
{
	return transfer_json;
}

std::unique_ptr<TransferableObject> UdpSocket::ReceiveType::object() const
{
	if (!is_object())
		return nullptr;
	try
	{
		auto transferable = TransferableObjectFabric::instance().get(transfer_json.id())();
		transferable->receiveJson(transfer_json.data());
		return std::move(transferable);
	}
	catch (std::out_of_range &err)
	{}
	return nullptr;
}
