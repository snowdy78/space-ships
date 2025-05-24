#include "game/EnemyShip.hpp"
#include "game/AbstractShip.hpp"
#include "game/GameManager.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/MoveShipAction.hpp"


EnemyShip::EnemyShip()
	: AbstractShip(*texture)
{
}

void EnemyShip::setTarget(AbstractShip *ship)
{
	target = ship;
}

const AbstractShip *EnemyShip::getTarget() const
{
	return target;
}

void EnemyShip::summonCopy(SpaceField &field) const
{
	field.summonShip<EnemyShip>();
}

rn::Vec2f EnemyShip::countMove() const
{
	if (!target)
		return {};
	if (!randomly_move)
		randomly_move = rn::random::integer(0, 1);
	float sign = *randomly_move
				 * rn::math::sgn(rn::math::length(target->getPosition() - getPosition()) - min_distance_to_target);
	rn::Vec2f dir{ getMoveDirection() * sign };

	return getAcceleration() * getVelocity() * dir;
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
	if (!target || !GameManager::exist())
		return;
	if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 0)
		randomly_move = std::nullopt;
	if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 3 == 1)
	{
		setMoveDirection(getDirection());
		GameManager::instance().action_manager.emplaceToTop<MoveShipAction>(TransferableActionProps{ this });
	}
	if (clock.getElapsedTime().asMilliseconds() % 1000 > 500)
	{
		setMoveDirection(Direction{ rn::math::nor(getDirection()) });
		GameManager::instance().action_manager.emplaceToTop<MoveShipAction>(TransferableActionProps{ this });
	}
}

void EnemyShip::update()
{
	AbstractShip::update();
	if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 1)
	{
		ready_to_shoot = true;
	}
	else if (clock.getElapsedTime().asMilliseconds() % 1000 > 500 && ready_to_shoot)
	{
		shoot();
		ready_to_shoot = false;
	}
}
