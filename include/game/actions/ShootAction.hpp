#pragma once

#include "coop/TransferableAction.hpp"

class ShootAction : public BaseTransferableAction<ShootAction>
{
public:
    constexpr static const char *direction = "direction";
    ShootAction(const TransferableActionProps &props = {});
    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;

private:
	Gun *m_gun{ nullptr };
	rn::Vec2f m_direction{};
};


