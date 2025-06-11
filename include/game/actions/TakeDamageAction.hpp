#pragma once

#include "coop/TransferableAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/Hittable.hpp"
#include "game/SpaceField.hpp"

class TakeDamageAction : public TransferActionBase<TakeDamageAction>
{
public:
	explicit TakeDamageAction(const TransferableActionProps &props = {});

    void play() override;
private:
    SpaceField::StatePtr<Hittable> m_hittable{}; // author
    SpaceField::StatePtr<DamageDealer> m_dealer{}; // contributor
};
