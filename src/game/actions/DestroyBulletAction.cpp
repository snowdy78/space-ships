#include "game/actions/DestroyBulletAction.hpp"
#include "game/GameManager.hpp"
#include "game/Hittable.hpp"

const size_t DestroyBulletAction::id = identify<DestroyBulletAction>();

DestroyBulletAction::DestroyBulletAction(GameObject *author, GameObject *contributor, const rn::Json &props)
	: TransferableAction(author, contributor, props)
{
	if (auto bullet = dynamic_cast<Bullet *>(author))
		m_bullet = bullet;
	if (auto hittable = dynamic_cast<Hittable *>(contributor))
		m_contributor = hittable;
}

void DestroyBulletAction::play()
{
	if (!m_bullet)
		return;
	if (GameManager::exist())
	{
		GameManager::instance().field.destroyBullet(m_bullet);
	}
}

Transferable::TransferJson DestroyBulletAction::toJson() const
{
	return
	{
		id
	};
}

AbstractAction *DestroyBulletAction::copy() const
{
	return new DestroyBulletAction(m_bullet, m_contributor);
}
