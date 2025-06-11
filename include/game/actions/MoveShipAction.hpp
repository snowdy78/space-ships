#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"
#include "game/SpaceField.hpp"

class MoveShipAction : public TransferActionBase<MoveShipAction>
{
public:
    constexpr static const char *md = "movement_direction";
    explicit MoveShipAction(const TransferableActionProps &props = {});

    void play() override;
    rn::Json toJson() const override;
private:
    SpaceField::StatePtr<AbstractShip> m_ship{}; // author
};
