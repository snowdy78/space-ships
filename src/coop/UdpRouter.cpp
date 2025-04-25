#include "coop\UdpRouter.hpp"
#include "coop/TransferableAction.hpp"
#include "coop/TransferableObject.hpp"


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

sf::Socket::Status UdpRouter::_send(const rn::Json &send_data)
{
	std::string json_string = encrypt(send_data.dump(-1, 0));
	sf::Packet packet;
	packet.append(json_string.c_str(), (json_string.size() + 1) * sizeof(char));
	Status status = UdpRouter::send(packet, ip, port);
	return status;
}

std::variant<rn::Json, sf::Socket::Status> UdpRouter::recieveJson()
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

sf::Socket::Status UdpRouter::sendObject(const TransferableObject *data)
{
	auto send_data	  = data->toJson();
	send_data["type"] = Response::TransferType::object;
	return _send(send_data);
}

sf::Socket::Status
UdpRouter::sendAction(std::optional<size_t> author_id, std::optional<size_t> target_id, TransferableAction *action)
{
	using namespace std::string_literals;
	if (!action)
		return sf::Socket::Status::Error;
	rn::Json send_data = action->toJson();
	send_data["type"]  = Response::TransferType::action;
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

std::variant<sf::Socket::Status, UdpRouter::Response> UdpRouter::recieve()
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
	if (type != Response::TransferType::action && type != Response::TransferType::object)
		throw incorrect_json_format("invalid type name");
	std::cout << "response body: " << json.dump(-1, 0) << "\n"; // TODO: remove
	return createResponseBody(json);
}
