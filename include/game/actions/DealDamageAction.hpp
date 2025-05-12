#pragma once

#include "coop/TransferableAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/Hittable.hpp"
#include "game/GameObject.hpp"

class DealDamageAction : public BaseTransferableAction<DealDamageAction>
{
public:
	explicit DealDamageAction(const TransferableActionProps &props = {});

    void play() override;
    TransferJson toJson() const override;
    AbstractAction *copy() const override;
private:
    DamageDealer *m_dealer{nullptr}; // author
    Hittable *m_hittable{nullptr}; // contributor
};
