#include "coop/ClientSocket.hpp"
#include <optional>
#include "coop/TransferObject.hpp"

ClientSocket::ClientSocket(uint16_t self_port, sf::IpAddress ip, uint16_t port)
    : sf::UdpSocket(), ip(ip), port(port)
{
    bind(self_port);
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
    packet.append(json_string.c_str(), json_string.size() * sizeof(char));
    Status status = UdpSocket::send(packet, ip, port);
    return status;
}

std::optional<TransferInstance> ClientSocket::recieve()
{
    sf::Packet packet;
    Status status = UdpSocket::receive(packet, ip, port);
    if (status != sf::Socket::Done)
        return std::nullopt;
    std::string str;
    size_t size = packet.getDataSize();
    char *p		= new char[size + 1];
    std::memcpy(p, packet.getData(), size);
    *(p + size) = '\0';
    str = p;
    delete[] p;
    std::cout << "\"" << str << "\"" << "\n";
    TransferInstance data(rn::Json::parse(decrypt(str)));
    std::optional<TransferInstance> result(data);
    return result;
}
