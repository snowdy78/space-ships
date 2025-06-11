#include "game/actions/DealDamageAction.hpp"

DealDamageAction::DealDamageAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (props.authorExist())
		m_dealer = *props.castAuthor<DamageDealer>();
	if (props.contributorExist())
		m_hittable = *props.castContributor<Hittable>();
}

void DealDamageAction::play()
{
	if (m_dealer.expired() || m_hittable.expired())
		return;
	auto dealer = m_dealer.lock();
	auto hittable_target = m_hittable.lock();
	dealer->dealDamage(hittable_target.get());
}
