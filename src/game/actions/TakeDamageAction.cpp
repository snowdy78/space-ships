#include "game/actions/TakeDamageAction.hpp"
#include "game/DamageDealer.hpp"
#include "game/SpaceFieldObject.hpp"

TakeDamageAction::TakeDamageAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (props.authorExist())
		m_hittable = *props.castAuthor<Hittable>();
	if (props.authorExist())
		m_dealer = *props.castContributor<DamageDealer>();
}

void TakeDamageAction::play()
{
	if (m_dealer.expired() && m_hittable.expired())
		return;
	auto dealer = m_dealer.lock();
	auto target = m_hittable.lock();
	target->takeDamage(dealer->getDamage());
	if (target->isDead())
	{
		if (auto space_field_obj = std::dynamic_pointer_cast<SpaceFieldObject>(target))
		{
			space_field_obj->destroy();
		}
	}
}
