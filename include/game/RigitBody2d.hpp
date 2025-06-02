#pragma once

#include "decl.hpp"

class Direction : public rn::Vec2f
{
public:
	explicit Direction() = default;
	explicit Direction(const rn::Vec2f &distance)
		: rn::Vec2f(rn::math::norm(distance))
	{}
	explicit Direction(float angle)
		: rn::Vec2f{ std::cos(angle), std::sin(angle) }
	{}
	const rn::Vec2f &toVector() const
	{
		return *this;
	}
	Direction &operator=(const rn::Vec2f &distance)
	{
		if (&distance != this)
		{
			rn::Vec2f &&direction = rn::math::norm(distance);
			x					  = direction.x;
			y					  = direction.y;
		}
		return *this;
	}
};
class RigitBody2d : public rn::MonoBehaviour
{
	float velocity = 5.f;
	Direction direction{};
	bool is_capable = true;
	struct PullForce
	{
		rn::Vec2f pull{0.0f, 0.0f};
		float force{0.0f};
	};
	PullForce pull_force;
	float mass{1.0};
	float m_acceleration{1.0f};
public:
	using Transformable::getPosition;

	virtual void rotation() {}
	virtual void movement() {}

	explicit RigitBody2d();

	bool isCapable() const;
	void setCapability(bool capability);
	void setPosition(const rn::Vec2f &p);
	void setPosition(float x, float y);
	void setDirection(float x, float y);
	void setDirection(const rn::Vec2f &p);
	void setVelocity(float velocity);
	void setAcceleration(float acceleration);
	float getAcceleration() const;
	virtual void onAccelerate() {}
	
	Direction getDirection() const;
	void move(float x, float y);
	float getMass() const;
	void setMass(float mass);
	void move(const rn::Vec2f &p);
	void setRotation(float angle);
	void rotate(float angle);
	virtual rn::Vec2f countMove() const = 0;

	virtual void onMove() {}
	virtual void onRotation() {}
	float getVelocity() const;
	void push(const rn::Vec2f &force);
	virtual void onPush() {}
	void setPulling(const rn::Vec2f &point, float force);

	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
