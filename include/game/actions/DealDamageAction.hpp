#pragma once

#include "coop/TransferableAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/Hittable.hpp"
#include "game/GameObject.hpp"

class DealDamageAction : public TransferableAction
{
public:
    explicit DealDamageAction(GameObject *author = nullptr, GameObject *contributor = nullptr, const rn::Json &props = {});

    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
    static const size_t id;
    DamageDealer *m_dealer{nullptr}; // author
    Hittable *m_hittable{nullptr}; // contributor
};
