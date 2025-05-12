#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"

class MoveShipAction : public BaseTransferableAction<MoveShipAction>
{
public:
    constexpr static const char *md = "movement_direction";
	explicit MoveShipAction(const TransferableActionProps &props = {});

    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
    AbstractShip *m_ship{nullptr}; // author
};
