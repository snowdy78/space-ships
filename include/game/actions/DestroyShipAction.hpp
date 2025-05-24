#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"
#include "game/actions/AbstractAction.hpp"

class DestroyShipAction : public TransferActionBase<DestroyShipAction>
{
public:
	DestroyShipAction(const TransferableActionProps &props = {});

    void play() override;
    rn::Json toJson() const override;
private:
    AbstractShip *m_ship;
    DamageDealer *m_destroyer{nullptr};
};