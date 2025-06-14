#include "game/EnemyShip.hpp"

#include "Helpers.hpp"
#include "game/AbstractShip.hpp"
#include "game/GameManager.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/MoveShipAction.hpp"
#include <list>

EnemyShip::EnemyShip()
	: AbstractShip(*texture)
{
}

void EnemyShip::setTarget(const SpaceField::StatePtr<AbstractShip> &target)
{
	m_target = target;
}

SpaceField::StatePtr<AbstractShip> EnemyShip::getTarget() const
{
	return m_target;
}

void EnemyShip::start()
{
	setVelocity(*velocity);
	movement_clock.start();
	shoot_clock.start();
	AbstractShip::start();
}

void EnemyShip::summonCopy(SpaceField &field) const
{
	field.summonShip<EnemyShip>();
}

rn::Vec2f EnemyShip::countMove() const
{
	if (m_target.expired())
		return {};
	using rn::math::number_t;
	using namespace std::chrono_literals;
	auto ship  = m_target.lock();
	float sign
		= static_cast<float>(rn::math::sgn(
			  rn::math::length(ship->getPosition() - getPosition()) - static_cast<number_t>(*min_target_distance)
		  ));
	rn::Vec2f dir{ getMoveDirection() * sign };
	
	return getVelocity() * dir;
}
void EnemyShip::rotation()
{
	AbstractShip::rotation();
	if (!m_target.expired())
	{
		auto ship = m_target.lock();
		setDirection(rn::math::norm(ship->getPosition() - getPosition()));
		setRotation(static_cast<float>(rn::math::rot(getDirection())));
	}
}
void EnemyShip::movement()
{
	using namespace std::chrono;
	using rn::math::rot, rn::math::norm, rn::math::nor;

	AbstractShip::movement();
	if (m_target.expired() || !GameManager::exist())
		return;

	if (timeStep(movement_clock, time_digit_type(*time_vertical_movement)))
	{
		if (!vertical)
		{
			vertical.moving(getDirection());
			setMoveDirection(norm(getMoveDirection() + vertical.moving()));
		}
	}
	else if (vertical)
	{
		auto a1 = rot(vertical.moving());
		auto a2 = rot(getMoveDirection());
		setMoveDirection(direction(a2 - a1));
		vertical.reset();
	}
	if (timeStep(movement_clock, time_digit_type(*time_horizontal_movement)))
	{
		if (!horizontal)
		{
			horizontal.moving(nor(getDirection()));
			setMoveDirection(norm(getMoveDirection() + horizontal.moving()));
		}
	}
	else if (horizontal)
	{
		auto a1 = rot(horizontal.moving());
		auto a2 = rot(getMoveDirection());
		setMoveDirection(direction(a2 - a1));
		horizontal.reset();
	}
	if (vertical || horizontal && existOnField())
	{
		GameManager::session()->action_manager.emplaceToTop<MoveShipAction>(TransferableActionProps{ self() });
	}
}

void EnemyShip::update()
{
	AbstractShip::update();
	if (everyTime(shoot_clock, time_digit_type(*shoot_time)))
	{
		shoot();
		shoot_clock.reset();
	}
}
