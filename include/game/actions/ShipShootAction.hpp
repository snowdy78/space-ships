#pragma once

#include "coop/TransferableAction.hpp"

class ShipShootAction : public TransferableAction 
{
    const static size_t id;
    AbstractShip *ship = nullptr;
    rn::Vec2f direction{};
public:
    ShipShootAction(GameObject *author = nullptr, GameObject *target = nullptr, const rn::Json &props = {});
    void play() override;
    virtual TransferJson toJson() const override;
    virtual AbstractAction *copy() const override;
};


