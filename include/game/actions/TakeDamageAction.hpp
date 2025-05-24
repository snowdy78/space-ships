#pragma once

#include "coop/TransferableAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/Hittable.hpp"
#include "game/GameObject.hpp"

class TakeDamageAction : public TransferActionBase<TakeDamageAction>
{
public:
	explicit TakeDamageAction(const TransferableActionProps &props = {});

    void play() override;
    rn::Json toJson() const override;
private:
    Hittable *m_hittable{nullptr}; // author
    DamageDealer *m_dealer{nullptr}; // contributor
};
