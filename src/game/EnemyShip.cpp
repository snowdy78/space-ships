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
	vertical_movement_clock.start();
	horizontal_movement_clock.start();
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
			  rn::math::length(ship->getPosition() - getPosition()) - static_cast<number_t>(min_distance_to_target)
		  ));
	rn::Vec2f dir{ getMoveDirection() * sign * (*randomly_move == 0 ? 1.f : -1.f) };
	
	return getVelocity() * dir;
}
void EnemyShip::rotation()
{
	AbstractShip::rotation();
	if (!m_target.expired())
	{
		auto ship = m_target.lock();
		setDirection(ship->getPosition() - getPosition());
		setRotation(rn::math::rot(getDirection()));
	}
}
void EnemyShip::movement()
{
	using namespace std::chrono;
	AbstractShip::movement();
	if (m_target.expired() || !GameManager::exist())
		return;
	if (timeStep(vertical_movement_clock, time_vertical_movement))
	{
		if (!randomly_move.has_value())
			randomly_move = rn::random::integer(0, 1);
		setMoveDirection(getDirection());
		GameManager::session()->action_manager.emplaceToTop<MoveShipAction>(TransferableActionProps{ this });
	}
	else
	{
		randomly_move = std::nullopt;
	}
	if (timeStep(horizontal_movement_clock, time_horizontal_movement))
	{
		if (!randomly_move.has_value())
			randomly_move = rn::random::integer(0, 1);
		setMoveDirection(rn::math::nor(getDirection()));
		GameManager::session()->action_manager.emplaceToTop<MoveShipAction>(TransferableActionProps{ this });
	}
	else
	{
		randomly_move = std::nullopt;
	}

}

void EnemyShip::update()
{
	AbstractShip::update();
	if (everyTime(shoot_clock, shoot_timing))
	{
		shoot();
		shoot_clock.reset();
	}
}
