#pragma once

#include "game/Collider.hpp"

class PolygonCollider : public Collider
{
	rn::math::polygon shape;

public:
	using shape_t = rn::math::polygon;
	PolygonCollider() {}
	PolygonCollider(rn::math::polygon shape)
		: shape(shape)
	{}

	virtual bool collide(const PolygonCollider &collider) const override;
	virtual bool collide(const EllipseCollider &collider) const override;
	void transform(const rn::math::polygon &polygon);
	
	const rn::math::shape &getColliderShape() const override;
	Collider *copy() const override;
	sf::FloatRect getBounds() const;
};
