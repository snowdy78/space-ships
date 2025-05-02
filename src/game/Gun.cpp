#include "game/Gun.hpp"
#include "Helpers.hpp"
#include "game/AbstractShip.hpp"
#include "game/GameGlobals.hpp"
#include "game/actions/ShootAction.hpp"


Gun::~Gun()
{
}

void Gun::startRollback()
{
	has_rollback = true;
	clock.start();
}
Gun::Gun(const AbstractShip *user)
	: ship(user)
{
}

void Gun::shoot()
{
	if (GameGlobals::exist() && !has_rollback)
	{
		auto direction{getTrajectory()};
		GameGlobals::instance().action_manager.emplaceToTop<ShootAction>(
			this, nullptr,
			rn::Json{
				{ "direction", { { "x", direction.x }, { "y", direction.y } } }
			}
		);
	}
}

void Gun::fire()
{
	if (GameGlobals::exist())
	{
		GameGlobals::instance().field.summonBullet(createBullet(), getTrajectory());
		onShoot();
		startRollback();
	}
}

bool Gun::hasRollback() const
{
	return has_rollback;
}

void Gun::update()
{
	if (everyTime(clock, getMillisDelay()))
	{
		clock.reset();
		has_rollback = false;
	}
}

float Gun::getMillisDelay() const
{
	return 0.3f;
}
