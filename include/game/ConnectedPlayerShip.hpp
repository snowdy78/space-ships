#pragma once

#include "PlayerShip.hpp"

class ConnectedPlayerShip : public PlayerShip
{
public:
    using PlayerShip::PlayerShip;

    void movement() override;
    void onEvent(sf::Event &event) override;
};
