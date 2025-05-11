#pragma once

#include "decl.hpp"
#include "RuneEngine/MenuBranch.hpp"
#include "components/Background.hpp"
#include "coop/TcpRouter.hpp"
#include "game/GameGlobals.hpp"

class ConnectToGameBranch : public rn::MenuBranch
{
    sf::IpAddress host_ip = sf::IpAddress::getLocalAddress();
    uint16_t host_port = 12345;
    sf::Text send_status{"", *Font::Default};
    sf::Text receive_status{"", *Font::Default};
    bool connected = false;
    bool window_focused = true;

    Background background{};
    GameGlobals *space = nullptr;
	GameGlobals::OnlineTraits *online = nullptr;

public:
	using rn::MenuBranch::MenuBranch;
    ~ConnectToGameBranch() override;
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
    void receivePackets() const;
};
