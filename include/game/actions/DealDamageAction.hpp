#pragma once

#include "coop/TransferableAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/Hittable.hpp"

class DealDamageAction : public TransferActionBase<DealDamageAction>
{
public:
    explicit DealDamageAction(const TransferableActionProps &props = {});

    void play() override;
    rn::Json toJson() const override;
private:
    DamageDealer *m_dealer{nullptr}; // author
    Hittable *m_hittable{nullptr}; // contributor
};
