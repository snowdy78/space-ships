#pragma once

#include "coop/TransferableAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/Hittable.hpp"
#include "game/GameObject.hpp"

class TakeDamageAction : public BaseTransferableAction<TakeDamageAction>
{
public:
    explicit TakeDamageAction(const TransferableActionProps &props = {});

    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
    Hittable *m_hittable{nullptr}; // author
    DamageDealer *m_dealer{nullptr}; // contributor
};
