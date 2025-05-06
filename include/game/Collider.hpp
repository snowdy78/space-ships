#pragma once

#include "decl.hpp"

class Collider : public sf::Transformable
{
public:
	virtual const rn::math::shape &getColliderShape() const		= 0;
	virtual sf::FloatRect getBounds() const				= 0;
	virtual bool collide(const PolygonCollider &collider) const = 0;
	virtual bool collide(const EllipseCollider &collider) const = 0;

	virtual Collider *copy() const = 0;
	// TODO add collider bounds
};
