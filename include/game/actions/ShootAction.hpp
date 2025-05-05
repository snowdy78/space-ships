#pragma once

#include "coop/TransferableAction.hpp"

class ShootAction : public TransferableAction 
{
    const static size_t id;
    Gun *m_gun{nullptr};
    rn::Vec2f m_direction{};
public:
    constexpr static const char *direction = "direction";
    ShootAction(GameObject *author = nullptr, GameObject *contributor = nullptr, const rn::Json &props = {});
    void play() override;
    virtual TransferJson toJson() const override;
    virtual AbstractAction *copy() const override;
};


