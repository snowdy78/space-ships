#include "game/AbstractWeapon.hpp"
#include "Helpers.hpp"
#include "game/actions/AbstractSummonAction.hpp"
#include "game/GameManager.hpp"
#include "game/actions/ShootAction.hpp"
#include "game/AbstractBullet.hpp"

AbstractWeapon::~AbstractWeapon() = default;

void AbstractWeapon::startRollback()
{
	has_rollback = true;
	clock.start();
}
AbstractWeapon::AbstractWeapon() = default;

void AbstractWeapon::shoot()
{
	if (GameManager::exist() && !has_rollback)
	{
		auto direction{ getTrajectory() };
		GameManager::session()->action_manager.emplaceToTop<ShootAction>(TransferableActionProps{
			this, nullptr, rn::Json{ { "direction", { { "x", direction.x }, { "y", direction.y } } } } });
	}
}

void AbstractWeapon::fire()
{
	if (GameManager::exist())
	{
		GameManager::session()->action_manager.emplaceToTop<SummonBulletAction>(
			bullet(), [this](AbstractBullet &bullet) {
				bullet.author = this;
				bullet.setPosition(getPosition());
				bullet.setDirection(getTrajectory());
			}
		);
		onShoot();
		startRollback();
	}
}

bool AbstractWeapon::hasRollback() const
{
	return has_rollback;
}

void AbstractWeapon::update()
{
	if (has_rollback && everyTime(clock, getRollback()))
	{
		clock.stop();
		clock.reset();
		has_rollback = false;
	}
}

std::chrono::milliseconds AbstractWeapon::getRollback() const
{
	return std::chrono::milliseconds{ 300 };
}
