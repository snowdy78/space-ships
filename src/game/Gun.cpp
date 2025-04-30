#include "game/Gun.hpp"
#include "Helpers.hpp"
#include "SoundDisperseEntity.hpp"
#include "game/AbstractShip.hpp"
#include "game/SpaceField.hpp"

Gun::~Gun() {}

void Gun::startRollback()
{
	has_rollback = true;
	clock.start();
}
Gun::Gun(const AbstractShip *user)
	: ship(user)
{
};

void Gun::shoot(const rn::Vec2f &direction)
{
	if (ship && ship->field && !has_rollback)
	{
		ship->field->summonBullet(createBullet(), getTrajectory());
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
