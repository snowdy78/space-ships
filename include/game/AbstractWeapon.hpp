#pragma once

#include "decl.hpp"
#include "game/SpaceItem.hpp"

class AbstractWeapon : public SpaceItem, public rn::LogicalObject, public sf::Transformable
{
	rn::Stopwatch clock;
	bool has_rollback = false;

protected:
	friend class AbstractBullet;
	void startRollback();

public:
	AbstractWeapon();
	~AbstractWeapon() override = 0;
	virtual void shoot();
	void fire();
	virtual AbstractWeapon *copy() const	= 0;
	virtual rn::Vec2f getTrajectory() const = 0;
	bool hasRollback() const;
	void update() override;
	virtual std::chrono::milliseconds getRollback() const;

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
