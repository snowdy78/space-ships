#pragma once

#include "decl.hpp"
#include "components/Background.hpp"
#include "game/GameManager.hpp"
#include "coop/TcpRouter.hpp"

class HostGameBranch : public rn::MenuBranch 
{
    sf::IpAddress ip_address = sf::IpAddress::getLocalAddress();
    uint16_t port = 12345;
    sf::Text send_status{"", *Font::Default};
    sf::Text receive_status{"", *Font::Default};
	bool connected = false;
    bool window_focused = true;

    Background background{};
    GameManager *space = nullptr;
	GameManager::OnlineTraits *online = nullptr;

public:
    using rn::MenuBranch::MenuBranch;
    ~HostGameBranch() override;
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
    void receivePackets() const;
};

