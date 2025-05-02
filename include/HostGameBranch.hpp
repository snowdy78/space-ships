#pragma once

#include "decl.hpp"
#include "components/Background.hpp"
#include "game/GameGlobals.hpp"
#include "coop/TcpRouter.hpp"

class HostGameBranch : public rn::MenuBranch 
{
    sf::IpAddress ip_address = sf::IpAddress::getLocalAddress();
    size_t port = 12345;
    TcpRouter client = TcpRouter(ip_address, port);
 
    sf::Text send_status{"", *Font::Default};
    sf::Text receive_status{"", *Font::Default};
	bool connected = false;
    bool window_focused = true;

    Background background{};
    GameGlobals *space = nullptr;
public:
    using rn::MenuBranch::MenuBranch;
    ~HostGameBranch() override;
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
    void receivePackets();
};

