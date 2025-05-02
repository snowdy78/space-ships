#include "game/actions/TakeDamageAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/SpaceFieldObject.hpp"

const size_t TakeDamageAction::id = identify<TakeDamageAction>();

TakeDamageAction::TakeDamageAction(GameObject *author, GameObject *contributor, const rn::Json &props)
	: TransferableAction(author, contributor, props)
{
	if (auto hittable = dynamic_cast<Hittable *>(author))
		m_hittable = hittable;
	if (auto dealer = dynamic_cast<DamageDealer *>(contributor))
		m_dealer = dealer;
}

void TakeDamageAction::play()
{
	if (!m_dealer && !m_hittable)
		return;
	m_hittable->takeDamage(m_dealer->getDamage());
	if (m_hittable->isDead())
	{
		if (auto space_field_obj = dynamic_cast<SpaceFieldObject *>(m_hittable))
		{
			space_field_obj->destroy();
		}
	}
}

Transferable::TransferJson TakeDamageAction::toJson() const
{
	return { id };
}

AbstractAction *TakeDamageAction::copy() const
{
    return new TakeDamageAction(m_hittable, m_dealer);
}
