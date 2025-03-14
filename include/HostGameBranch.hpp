#pragma once

#include "decl.hpp"

class HostGameBranch : public rn::MenuBranch 
{
public:
    using rn::MenuBranch::MenuBranch;
    void start() override;
    void update() override;
    void onEvent(sf::Event &event) override;
};

