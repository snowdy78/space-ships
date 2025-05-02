#include "game/actions/DealDamageAction.hpp"

const size_t DealDamageAction::id = identify<DealDamageAction>();

DealDamageAction::DealDamageAction(GameObject *author, GameObject *contributor, const rn::Json &props)
	: TransferableAction(author, contributor, props)
{
	if (auto dealer = dynamic_cast<DamageDealer *>(author))
		m_dealer = dealer;
	if (auto hittable = dynamic_cast<Hittable *>(contributor))
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
	return { id };
}

AbstractAction *DealDamageAction::copy() const
{
    return new DealDamageAction(m_dealer, m_hittable);
}
