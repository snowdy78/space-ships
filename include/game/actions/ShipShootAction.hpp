#pragma once

#include "coop/TransferableAction.hpp"

class ShipShootAction : public TransferableAction 
{
    const static size_t id; 
public:
    ShipShootAction(std::optional<GameObject *> author = std::nullopt, std::optional<GameObject *> target = std::nullopt, const rn::Json &props = {});
    void play() override;
    virtual TransferJson toJson() const override;
    virtual AbstractAction *copy() const override;
};


