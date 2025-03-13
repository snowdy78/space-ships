#include "game/colliders/EllipseCollider.hpp"
#include "game/colliders/PolygonCollider.hpp"

const rn::math::shape &EllipseCollider::getColliderShape() const
{
	return shape;
}

bool EllipseCollider::collide(const PolygonCollider &collider) const
{
	return shape.collide(static_cast<const rn::math::polygon &>(collider.getColliderShape()));
}
void EllipseCollider::transform(const rn::math::ellipse &ellipse)
{
	shape = ellipse;
}

bool EllipseCollider::collide(const EllipseCollider &collider) const
{
	rn::Circle circle = rn::Circle(shape.r);
	circle.setScale(shape.a, shape.b);
	circle.setPosition(shape.center);
	circle.setRotation(shape.rotation);
	circle.setPointCount(25);
	return rn::math::polygon(circle).collide(static_cast<const rn::math::ellipse &>(collider.getColliderShape()));
}

Collider *EllipseCollider::copy() const
{
	return new EllipseCollider(shape);
}

sf::Drawable *EllipseCollider::toDrawable() const
{
	rn::Circle *circle = new rn::Circle(shape.r);
	circle->setScale(shape.a, shape.b);
	circle->setPosition(shape.center);
	circle->setRotation(shape.rotation);
	return circle;
}
