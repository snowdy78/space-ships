#pragma once

#include "coop/TransferableAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/Hittable.hpp"
#include "game/GameObject.hpp"

class TakeDamageAction : public TransferableAction
{
public:
    explicit TakeDamageAction(GameObject *author = nullptr, GameObject *contributor = nullptr, const rn::Json &props = {});

    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
    static const size_t id;
    Hittable *m_hittable{nullptr}; // author
    DamageDealer *m_dealer{nullptr}; // contributor
};
