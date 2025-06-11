#pragma once

#include "coop/TransferableAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/Hittable.hpp"
#include "game/SpaceField.hpp"

class DealDamageAction : public TransferActionBase<DealDamageAction>
{
public:
    explicit DealDamageAction(const TransferableActionProps &props = {});

    void play() override;
private:
    SpaceField::StatePtr<DamageDealer> m_dealer{}; // author
	SpaceField::StatePtr<Hittable> m_hittable{}; // contributor
};
