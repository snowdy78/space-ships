#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"
#include "game/actions/AbstractAction.hpp"

class DestroyShipAction : public TransferableAction
{
public:
    DestroyShipAction(GameObject *author = nullptr, GameObject *contributor = nullptr, const rn::Json &props = {});

    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
    static const size_t id;
    AbstractShip *m_ship;
    DamageDealer *m_destroyer{nullptr};
};