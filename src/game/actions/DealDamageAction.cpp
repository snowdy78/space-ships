#include "game/actions/DealDamageAction.hpp"

DealDamageAction::DealDamageAction(const TransferableActionProps &props)
	: BaseTransferableAction(props)
{
	if (auto dealer = dynamic_cast<DamageDealer *>(props.author))
		m_dealer = dealer;
	if (auto hittable = dynamic_cast<Hittable *>(props.contributor))
		m_hittable = hittable;
}

void DealDamageAction::play()
{
	if (!m_dealer && !m_hittable)
		return;
	m_dealer->dealDamage(m_hittable);
}

Transferable::TransferJson DealDamageAction::toJson() const
{
	return { id() };
}

AbstractAction *DealDamageAction::copy() const
{
    return new DealDamageAction({m_dealer, m_hittable});
}
