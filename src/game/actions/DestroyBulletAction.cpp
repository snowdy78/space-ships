#include "game/actions/DestroyBulletAction.hpp"
#include "game/GameGlobals.hpp"
#include "game/Hittable.hpp"

DestroyBulletAction::DestroyBulletAction(const TransferableActionProps &props)
	: BaseTransferableAction(props)
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
	if (GameGlobals::exist())
		GameGlobals::instance().field.destroyBullet(m_bullet);
}

Transferable::TransferJson DestroyBulletAction::toJson() const
{
	return { id() };
}

AbstractAction *DestroyBulletAction::copy() const
{
	return new DestroyBulletAction({ m_bullet, m_contributor });
}
