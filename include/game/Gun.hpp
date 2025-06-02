#pragma once

#include "SoundDisperseEntity.hpp"
#include "decl.hpp"
#include "game/GameObject.hpp"

class Gun : public sf::Transformable, public rn::LogicalObject, public virtual GameObject
{
	rn::Stopwatch clock;
	bool has_rollback = false;

protected:

	friend class Bullet;
	void startRollback();

public:
	const AbstractShip *user = nullptr;
	Gun();
	~Gun() override = 0;
	virtual void shoot();
	void fire();
	virtual Gun *copy() const				= 0;
	virtual rn::Vec2f getTrajectory() const = 0;
	bool hasRollback() const;
	void update() override;
	virtual float getMillisDelay() const;

protected:
	/**
	 * @brief should return the id of a bullet
	 * @return 
	 */
	virtual size_t bullet() const = 0;
	virtual void onShoot()
	{
	}
};
