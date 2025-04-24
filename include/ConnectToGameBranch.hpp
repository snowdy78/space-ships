#pragma once

#include "RuneEngine/MenuBranch.hpp"
#include "SFML/Network/IpAddress.hpp"
#include "components/Background.hpp"
#include "decl.hpp"
#include "coop\UdpSocket.hpp"
#include "game/GameGlobals.hpp"

class ConnectToGameBranch : public rn::MenuBranch
{
    sf::IpAddress ip_address = sf::IpAddress::getLocalAddress();
    size_t port = 12345;
    size_t remote_port = 12344;
    UdpSocket client = UdpSocket(ip_address, port);
    sf::Text send_status{"", *Font::Default};
    sf::Text receive_status{"", *Font::Default};
    bool connected = false;

    Background background{};
    GameGlobals *space = nullptr;
public:
	using rn::MenuBranch::MenuBranch;
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
    void receivePackets();
};
