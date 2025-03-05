#pragma once

#include "decl.hpp"
#include "coop/ClientSocket.hpp"
#include <optional>

class ServerTestBranch : public rn::MenuBranch
{
    std::optional<ClientSocket> client;
public:
    ServerTestBranch(sf::RenderWindow &window) : rn::MenuBranch(window) {}
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
};
