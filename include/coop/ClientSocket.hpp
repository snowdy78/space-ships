#pragma once

#include "coop/TransferInstance.hpp"
#include "coop/TransferObject.hpp"

class ClientSocket : public sf::UdpSocket
{
    sf::IpAddress ip;
    uint16_t port;
    TransferInstance data;

    static std::string encrypt(const std::string &str);
    static std::string decrypt(const std::string &str);

    using UdpSocket::send;
    using UdpSocket::receive;
public:
    ClientSocket(sf::IpAddress remote_ip, uint16_t remote_port = sf::Socket::AnyPort);
    sf::Socket::Status send(const TransferObject *data);
    std::optional<TransferInstance> recieve();
};
