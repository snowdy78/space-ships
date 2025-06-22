#pragma once

#include "AbstractShip.hpp"
#include "Helpers.hpp"
#include "components/FileLoader.hpp"
#include "decl.hpp"
#include "game/GameObjectBase.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/MoveShipAction.hpp"
#include "tools/AssignWeapon.hpp"

#ifndef ENEMY_TEST_COLLISION
// #define ENEMY_TEST_COLLISION
#endif
template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
class EnemyShipBase : public AbstractShip, public GameObjectBase<Derived>
{
	using time_digit_type					   = std::chrono::milliseconds;
	inline static loading<sf::Texture> texture = TextureLoader::instance().addToUpload(PathSprite).get();
	struct props
	{
		inline static auto &self_config = config::instance().get(ClassName);
		G_CONFIG_PROP_DEFINE(self_config, velocity);
		G_CONFIG_PROP_DEFINE(self_config, min_target_distance);
		G_CONFIG_PROP_DEFINE(self_config, time_vertical_movement);
		G_CONFIG_PROP_DEFINE(self_config, time_horizontal_movement);
		G_CONFIG_PROP_DEFINE(self_config, shoot_time);
	};
	time_digit_type time_vertical_movement{ *props::time_vertical_movement },
		time_horizontal_movement{ *props::time_horizontal_movement }, shoot_time{ *props::shoot_time };
	rn::Stopwatch movement_clock;
	rn::Stopwatch shoot_clock{ time_digit_type{ 200 } + time_digit_type{ rn::random::integer(0, 400) } };
	SpaceField::StatePtr<AbstractShip> m_target;
	class random_move
	{
	public:
		random_move() = default;
		void moving(const rn::Vec2f &direction)
		{
			m_direction = direction * (rn::random::chance(0.5) ? -1.f : 1.f);
		}
		void reset()
		{
			m_direction = std::nullopt;
		}
		rn::Vec2f moving() const
		{
			return *m_direction;
		}
		operator bool() const
		{
			return m_direction.has_value();
		}

	private:
		std::optional<rn::Vec2f> m_direction = std::nullopt;
	};
	random_move vertical;
	random_move horizontal;

public:
	using destroy_target_callback_type = std::function<void()>;
	EnemyShipBase();
	~EnemyShipBase() override = default;
	void setTarget(const SpaceField::StatePtr<AbstractShip> &target);
	SpaceField::StatePtr<AbstractShip> getTarget() const;
	void start() override;
	void update() override;
	void movement() override;
	virtual void shootBehaviour();
	void rotation() override;
	void summonCopy(SpaceField &field) const override;
	rn::Vec2f countMove() const override;
#ifdef ENEMY_TEST_COLLISION
	void onCollisionEnter(const Collidable *collidable) override
	{
		AbstractShip::onCollisionEnter(collidable);
		update_counter = 0;
		end_counter	   = 0;
		std::cout << this << "collision enter" << ++enter_counter << "\n";
	}
	void onCollisionUpdate(const Collidable *collidable) override
	{
		AbstractShip::onCollisionUpdate(collidable);
		enter_counter = 0;
		end_counter	  = 0;
		std::cout << this << "collision update" << ++update_counter << "\n";
	}
	void onCollisionEnd(const Collidable *collidable) override
	{
		AbstractShip::onCollisionEnd(collidable);
		update_counter = 0;
		enter_counter  = 0;
		std::cout << this << "collision end" << ++end_counter << "\n";
	}

private:
	int enter_counter = 0, update_counter = 0, end_counter = 0;
#endif
};
template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::EnemyShipBase()
	: AbstractShip(*texture)
{
}
template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
void EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::setTarget(const SpaceField::StatePtr<AbstractShip> &target)
{
	m_target = target;
}

template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
SpaceField::StatePtr<AbstractShip> EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::getTarget() const
{
	return m_target;
}

template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
void EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::start()
{
	this->setVelocity(*props::velocity);
	movement_clock.start();
	shoot_clock.start();
	::makeWeaponForShip<Weapon>(*this);
	AbstractShip::start();
}
template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
void EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::summonCopy(SpaceField &field) const
{
	field.summonShip<Derived>();
}

template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
rn::Vec2f EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::countMove() const
{
	if (m_target.expired())
		return {};
	using rn::math::number_t;
	using namespace std::chrono_literals;
	auto ship  = m_target.lock();
	float sign = static_cast<float>(rn::math::sgn(
		rn::math::length(ship->getPosition() - getPosition()) - static_cast<number_t>(*props::min_target_distance)
	));
	if (sign < 0)
		sign = 0;
	rn::Vec2f dir{ getMoveDirection() * sign };

	return getVelocity() * dir;
}
template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
void EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::rotation()
{
	AbstractShip::rotation();
	if (!m_target.expired())
	{
		auto ship = m_target.lock();
		setDirection(rn::math::norm(ship->getPosition() - getPosition()));
		setRotation(static_cast<float>(rn::math::rot(getDirection())));
	}
}
template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
void EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::movement()
{
	using namespace std::chrono;
	using rn::math::rot, rn::math::norm, rn::math::nor;
	constexpr float max_distance_random_move = 500;
	AbstractShip::movement();
	if (m_target.expired() || !GameManager::exist())
		return;
	auto target = m_target.lock();
	if (rn::math::length(target->getPosition() - getPosition()) > max_distance_random_move) // set moving to target if target too far from enemy
	{
		setMoveDirection(norm(target->getPosition() - getPosition()));
		GameManager::session()->action_manager.emplaceToTop<MoveShipAction>(TransferableActionProps{ self() });
		return;
	}

	if (timeStep(movement_clock, time_vertical_movement))
	{
		if (!vertical)
		{
			vertical.moving(getDirection());
			this->setMoveDirection(norm(getMoveDirection() + vertical.moving()));
		}
	}
	else if (vertical)
	{
		auto a1 = rot(vertical.moving());
		auto a2 = rot(getMoveDirection());
		this->setMoveDirection(direction(a2 - a1));
		vertical.reset();
	}
	if (timeStep(movement_clock, time_horizontal_movement))
	{
		if (!horizontal)
		{
			horizontal.moving(nor(getDirection()));
			this->setMoveDirection(norm(getMoveDirection() + horizontal.moving()));
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
		GameManager::session()->action_manager.emplaceToTop<MoveShipAction>(TransferableActionProps{ self() });
}

template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
void EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::shootBehaviour()
{
	if (everyTime(shoot_clock, shoot_time))
	{
		shoot();
		shoot_clock.reset();
	}
}

template<class Derived, class Weapon, char const *ClassName, char const *PathSprite>
void EnemyShipBase<Derived, Weapon, ClassName, PathSprite>::update()
{
	AbstractShip::update();
	shootBehaviour();
}
