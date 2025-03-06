#include "coop/ClientSocket.hpp"
#include <optional>
#include "coop/TransferObject.hpp"

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

sf::Socket::Status ClientSocket::send(const TransferObject *data)
{
    std::string json_string = encrypt(data->toJson().dump(-1, 0));
    sf::Packet packet;
    char *p = new char[json_string.size() + 1];
    std::memcpy(p, json_string.c_str(), json_string.size());
    *(p + json_string.size()) = '\0';
    packet.append(p, (json_string.size() + 1) * sizeof(char));
    Status status = UdpSocket::send(packet, ip, port);
    return status;
}

std::optional<TransferInstance> ClientSocket::recieve()
{
    sf::Packet packet;
    sf::IpAddress ip2 = ip;
    auto port2 = port;
    Status status = UdpSocket::receive(packet, ip2, port2);
    if (status != sf::Socket::Done)
        return std::nullopt;
    std::string str{ static_cast<const char *>(packet.getData())};
    std::cout << str << "\n";
    rn::Json json = rn::Json::parse(decrypt(str));
    std::cout << json << "\n";
    TransferInstance instance = json;
    std::optional<TransferInstance> data(json);
    std::cout << *data << "\n";
    return data;
}
