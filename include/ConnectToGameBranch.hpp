#pragma once

#include "decl.hpp"
#include "RuneEngine/MenuBranch.hpp"
#include "components/Background.hpp"
#include "coop/TcpRouter.hpp"
#include "game/GameGlobals.hpp"

class ConnectToGameBranch : public rn::MenuBranch
{
    sf::IpAddress host_ip = sf::IpAddress::getLocalAddress();
    size_t host_port = 12345;
    TcpRouter client = TcpRouter(host_ip, host_port);
    sf::Text send_status{"", *Font::Default};
    sf::Text receive_status{"", *Font::Default};
    bool connected = false;
    bool window_focused = true;

    Background background{};
    GameGlobals *space = nullptr;
public:
	using rn::MenuBranch::MenuBranch;
    ~ConnectToGameBranch() override;
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
    void receivePackets();
};
