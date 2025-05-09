#include "game/RigitBody2d.hpp"

RigitBody2d::RigitBody2d() {}

void RigitBody2d::setPosition(const rn::Vec2f &p)
{
	sf::Transformable::setPosition(p);
	onMove();
}

void RigitBody2d::setPosition(float x, float y)
{
	sf::Transformable::setPosition(x, y);
	onMove();
}

void RigitBody2d::move(float x, float y)
{
	sf::Transformable::move(x, y);
	onMove();
}

void RigitBody2d::move(const rn::Vec2f &p)
{
	sf::Transformable::move(p);
	onMove();
}

void RigitBody2d::update()
{
	if (!isCapable()) 
		return;
	movement();
	rotation();
}

void RigitBody2d::onEvent(sf::Event &event) {}

void RigitBody2d::draw(sf::RenderTarget &target, sf::RenderStates states) const {}
float RigitBody2d::getVelocity() const
{
	return velocity;
}
void RigitBody2d::start() {}
void RigitBody2d::setRotation(float angle)
{
	sf::Transformable::setRotation(angle);
	onRotation();
}

void RigitBody2d::rotate(float angle)
{
	sf::Transformable::rotate(angle);
	onRotation();
}
void RigitBody2d::setDirection(const rn::Vec2f &p)
{
	direction = p;
}
void RigitBody2d::setDirection(float x, float y)
{
	direction.x = x;
	direction.y = y;
}
Direction RigitBody2d::getDirection() const
{
	return direction;
}
void RigitBody2d::setVelocity(float velocity)
{
	this->velocity = velocity;
}
bool RigitBody2d::isCapable() const
{
	return is_capable;
}
void RigitBody2d::setCapability(bool capability)
{
	is_capable = capability;
}
void RigitBody2d::setAcceleration(float acceleration)
{
	m_acceleration = acceleration;
	onAccelerate();
}
float RigitBody2d::getAcceleration() const
{
	return m_acceleration;
}
void RigitBody2d::push(const rn::Vec2f &force)
{
	using rn::math::length;
	rn::Vec2f v{ velocity * direction };
	velocity  = length(v + force);
	direction = rn::math::norm(v + force);
	setAcceleration(0.f);
	onPush();
}
void RigitBody2d::setPulling(const rn::Vec2f &point, float force)
{
	pull_force.pull	 = point;
	pull_force.force = force;
}
