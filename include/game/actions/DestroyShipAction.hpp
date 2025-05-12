#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"
#include "game/actions/AbstractAction.hpp"

class DestroyShipAction : public BaseTransferableAction<DestroyShipAction>
{
public:
	DestroyShipAction(const TransferableActionProps &props = {});

    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
    AbstractShip *m_ship{nullptr};
    DamageDealer *m_destroyer{nullptr};
};