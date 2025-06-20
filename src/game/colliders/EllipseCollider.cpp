#include "game/colliders/EllipseCollider.hpp"
#include "game/colliders/PolygonCollider.hpp"

const rn::math::shape &EllipseCollider::getColliderShape() const
{
	return shape;
}

bool EllipseCollider::collide(const PolygonCollider &collider) const
{
	return shape.collide(dynamic_cast<const rn::math::polygon &>(collider.getColliderShape()));
}
void EllipseCollider::transform(const rn::math::ellipse &ellipse)
{
	shape = ellipse;
}

bool EllipseCollider::collide(const EllipseCollider &collider) const
{
	return shape.collide(dynamic_cast<const rn::math::ellipse &>(collider.getColliderShape()));
}

Collider *EllipseCollider::copy() const
{
	return new EllipseCollider(shape);
}

sf::FloatRect EllipseCollider::getBounds() const 
{
	rn::Circle circle(shape.r);
	circle.setScale(shape.a, shape.b);
	circle.setPosition(shape.center);
	circle.setOrigin(shape.r, shape.r);
	circle.setRotation(shape.rotation);
	return circle.getGlobalBounds();
}
