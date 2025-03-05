#include "coop/ClientSocket.hpp"

ClientSocket::ClientSocket(sf::IpAddress ip, uint16_t port)
    : sf::UdpSocket(), ip(ip)
{
    bind(port);
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

bool ClientSocket::send(const TransferObject *data)
{
    std::string json_string = encrypt(data->toJson().dump(-1, 0));
    sf::Packet packet;
    packet.append(json_string.c_str(), json_string.size() * sizeof(char));
    Status status = UdpSocket::send(packet, ip, port);
    return status == sf::Socket::Done;
}

TransferInstance ClientSocket::recieve()
{
    sf::Packet packet;
    Status status = UdpSocket::receive(packet, ip, port);
    std::string str;
    size_t size = packet.getDataSize();
    char *p		= new char[size + 1];
    std::memcpy(p, packet.getData(), size);
    *(p + size) = '\0';
    str = p;
    delete[] p;
    TransferInstance data(rn::Json::parse(decrypt(str)));
    return data;
}
