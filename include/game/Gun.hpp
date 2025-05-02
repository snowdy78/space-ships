#pragma once

#include "SoundDisperseEntity.hpp"
#include "decl.hpp"
#include "game/GameObject.hpp"

class Gun : public sf::Transformable, public rn::LogicalObject, public GameObject
{
	rn::Stopwatch clock;
	bool has_rollback = false;

protected:
	const AbstractShip *ship;

	friend class Bullet;
	void startRollback();

public:
	const AbstractShip *const &user = ship;
	Gun(const AbstractShip *user);
	virtual ~Gun() = 0;
	virtual void shoot();
	void fire();
	virtual Gun *copy() const				= 0;
	virtual rn::Vec2f getTrajectory() const = 0;
	bool hasRollback() const;
	void update() override;
	virtual float getMillisDelay() const;
protected:
	virtual Bullet *createBullet() const	= 0;
	virtual void onShoot() {}
};
