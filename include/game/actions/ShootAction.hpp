#pragma once

#include "coop/TransferableAction.hpp"

class ShootAction : public TransferActionBase<ShootAction> 
{
    AbstractWeapon *m_gun{nullptr};
    rn::Vec2f m_direction{};
public:
    constexpr static const char *direction = "direction";
	ShootAction(const TransferableActionProps &props = {});
    void play() override;
    rn::Json toJson() const override;
};


