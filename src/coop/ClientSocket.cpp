#include "coop/ClientSocket.hpp"
#include "coop/TransferableAction.hpp"
#include "coop/Transferable.hpp"
#include "coop/TransferableFabric.hpp"
#include "coop/TransferableActionFabric.hpp"
#include "game/GameObjectFabric.hpp"

ClientSocket::ClientSocket(sf::IpAddress ip, uint16_t port)
    : sf::UdpSocket(), ip(ip), port(port)
{
}

std::string ClientSocket::encrypt(const std::string &str)
{
    // TODO: encryption of data
    return str;
}

std::string ClientSocket::decrypt(const std::string &str)
{
    // TODO: decryption of data
    return str;
}

sf::Socket::Status ClientSocket::_send(const rn::Json &send_data) 
{
    std::string json_string = encrypt(send_data.dump(-1, 0));
    sf::Packet packet;
    packet.append(json_string.c_str(), (json_string.size() + 1) * sizeof(char));
    Status status = UdpSocket::send(packet, ip, port);
    return status;
}

std::variant<rn::Json, sf::Socket::Status> ClientSocket::recieveJson() 
{
    sf::Packet packet;
    sf::IpAddress ip2 = ip;
    auto port2 = port;
    Status status = UdpSocket::receive(packet, ip2, port2);
    if (status != sf::Socket::Done)
        return status;
    std::string str{ static_cast<const char *>(packet.getData())};
    rn::Json json = rn::Json::parse(decrypt(str));
    return json;
}

sf::Socket::Status ClientSocket::send(const Transferable *data)
{
    rn::Json send_data;
    send_data["type"] = data->type();
    send_data["data"] = data->toJson();
    return _send(send_data);
}

sf::Socket::Status ClientSocket::sendAction(std::optional<size_t> author_id, std::optional<size_t> target_id, TransferableAction *action) 
{
    using namespace std::string_literals;
    if (!action)
        return sf::Socket::Status::Error;
    rn::Json send_data;
    send_data["type"] = action->type();
    if (author_id)
        send_data["author_id"] = *author_id;
    else
        send_data["author_id"] = nullptr;
    if (target_id)
        send_data["target_id"] = *target_id;
    else
        send_data["target_id"] = nullptr;
    send_data["props"] = action->toJson();

    return _send(send_data);
}

std::variant<sf::Socket::Status, ClientSocket::ReceiveType> ClientSocket::recieve()
{
    auto json_or_status = recieveJson();
    if (std::holds_alternative<sf::Socket::Status>(json_or_status))
    {
        return std::get<sf::Socket::Status>(json_or_status);
    }
    auto &json = std::get<rn::Json>(json_or_status);
    if (!json || !json.contains("type") || !json.at("type").is_string())
        throw incorrect_json_format("invalid json format");
    auto type = json.at("type");
    if (type != Transferable::action && type != Transferable::object)
        throw incorrect_json_format("invalid type name");
    return ClientSocket::ReceiveType(json);
}

ClientSocket::ReceiveType::ReceiveType(const rn::Json &data_json)
	: data_json(data_json)
{}

const Transferable::Type &ClientSocket::ReceiveType::type() const
{
	return data_json.at("type") == Transferable::action ? Transferable::action : Transferable::object;
}

TransferableAction *ClientSocket::ReceiveType::action() const 
{
    auto author = GameObjectFabric::instance().get(data_json["author_id"]);
    auto target = GameObjectFabric::instance().get(data_json["target_id"]);
    return TransferableActionFabric::instance().get(data_json["type"])(author, target, data_json["props"]);
}

Transferable *ClientSocket::ReceiveType::object() const
{
    return TransferableFabric::instance().get(data_json["id"])();
}