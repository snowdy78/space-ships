#pragma once

#include "coop/TransferableAction.hpp"

class ShipShootAction : public TransferableAction 
{
    inline const static size_t id = identify<ShipShootAction>(); 
public:
    ShipShootAction();
    void play() override;
    virtual rn::Json toJson() const override;
    virtual AbstractAction *copy() const override;
};
