#pragma once

#include "coop/TransferableAction.hpp"
#include "game/SpaceField.hpp"

class ShootAction : public TransferActionBase<ShootAction> 
{
public:
    constexpr static const char *direction = "direction";
	ShootAction(const TransferableActionProps &props = {});
    void play() override;
    rn::Json toJson() const override;
private:
	SpaceField::StatePtr<AbstractWeapon> m_gun{};
	rn::Vec2f m_direction{};
};


