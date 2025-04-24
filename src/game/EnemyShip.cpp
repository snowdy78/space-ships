#include "game/EnemyShip.hpp"
#include "game/AbstractShip.hpp"
#include "game/SpaceField.hpp"

EnemyShip::EnemyShip()
	: AbstractShip(*texture)
{}

void EnemyShip::setTarget(AbstractShip *ship)
{
	target = ship;
}

const AbstractShip *EnemyShip::getTarget() const
{
	return target;
}


void EnemyShip::onMove()
{
	AbstractShip::onMove();
	updateCollider();
}
void EnemyShip::onHit()
{
	AbstractShip::onHit();
}

void EnemyShip::summonCopy(SpaceField *field) const 
{
	field->appendShip<EnemyShip>();
}

AbstractShip *EnemyShip::copy() const
{
	return new EnemyShip();
}
void EnemyShip::rotation()
{
	AbstractShip::rotation();
	if (target)
	{
		setDirection(target->getPosition() - getPosition());
		setRotation(rn::math::rot(getDirection()));
	}
}
void EnemyShip::movement()
{
	AbstractShip::movement();
	if (!target)
		return;
	if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 0)
	{
		randomly_move = nullptr;
	}
	if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 3 == 1)
	{
		if (!randomly_move)
		{
			randomly_move.reset(new int(rn::random::integer(0, 1)));
		}
		
		*randomly_move *= rn::math::sgn(rn::math::length(target->getPosition() - getPosition()) - min_distance_to_target);
		move(*randomly_move * getVelocity() * getDirection());
	}
	if (static_cast<int>(clock.getElapsedTime().asMilliseconds()) % 1000 > 500)
	{
		if (!randomly_move)
		{
			randomly_move.reset(new int(rn::random::integer(0, 1)));
		}
		move(*randomly_move * getVelocity() * rn::math::nor(getDirection()));
	}
}

void EnemyShip::update()
{
	AbstractShip::update();
	if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 1)
	{
		ready_to_shoot = true;
	}
	else if (static_cast<int>(clock.getElapsedTime().asMilliseconds()) % 1000 > 500 && ready_to_shoot)
	{
		shoot();
		ready_to_shoot = false;
	}
}
