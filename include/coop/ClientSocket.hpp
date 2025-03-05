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
    using UdpSocket::bind;
    using UdpSocket::unbind;
public:
    ClientSocket(uint16_t self_port, sf::IpAddress remote_ip, uint16_t remote_port);
    bool send(const TransferObject *data);
    TransferInstance recieve();
};
