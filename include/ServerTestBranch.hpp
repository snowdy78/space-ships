#pragma once

#include "decl.hpp"
#include "coop/ClientSocket.hpp"
#include <optional>

class ServerTestBranch : public rn::MenuBranch
{
    std::optional<ClientSocket> client;
    rn::Button send_button{{}, "Send", *Font::Default};
    sf::Text send_status{"", *Font::Default};
    sf::Text receive_status{"", *Font::Default};
public:
    ServerTestBranch(sf::RenderWindow &window) : rn::MenuBranch(window) {}
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
};
