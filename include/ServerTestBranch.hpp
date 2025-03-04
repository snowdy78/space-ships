#pragma once

#include "decl.hpp"
#include "coop/Server.hpp"
#include "coop/Client.hpp"

class ServerTestBranch : public rn::MenuBranch
{
public:
    ServerTestBranch(sf::RenderWindow &window) : rn::MenuBranch(window) {}
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
};
