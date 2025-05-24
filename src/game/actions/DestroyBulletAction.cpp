#include "game/actions/DestroyBulletAction.hpp"
#include "game/GameManager.hpp"
#include "game/Hittable.hpp"

DestroyBulletAction::DestroyBulletAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	if (auto bullet = dynamic_cast<Bullet *>(props.author))
		m_bullet = bullet;
	if (auto hittable = dynamic_cast<Hittable *>(props.contributor))
		m_contributor = hittable;
}

void DestroyBulletAction::play()
{
	if (!m_bullet)
		return;
	if (GameManager::exist())
		GameManager::session()->field.destroyBullet(m_bullet);
}

rn::Json DestroyBulletAction::toJson() const
{
	return {};
}
