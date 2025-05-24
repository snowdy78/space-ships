#include "game/actions/TakeDamageAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/SpaceFieldObject.hpp"

TakeDamageAction::TakeDamageAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (auto hittable = dynamic_cast<Hittable *>(getAuthor()))
		m_hittable = hittable;
	if (auto dealer = dynamic_cast<DamageDealer *>(getContributor()))
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

rn::Json TakeDamageAction::toJson() const
{
	return { };
}
