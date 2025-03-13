#include "game/colliders/PolygonCollider.hpp"
#include "game/colliders/EllipseCollider.hpp"

bool PolygonCollider::collide(const PolygonCollider &collider) const
{
	return shape.collide(collider.shape);
}

bool PolygonCollider::collide(const EllipseCollider &collider) const
{
	return shape.collide(static_cast<const rn::math::ellipse &>(collider.getColliderShape()));
}
void PolygonCollider::transform(const rn::math::polygon &polygon)
{
	shape = polygon;
}
Collider *PolygonCollider::copy() const
{
	return new PolygonCollider(shape);
}
const rn::math::shape &PolygonCollider::getColliderShape() const
{
	return shape;
}
sf::Drawable *PolygonCollider::toDrawable() const
{
	rn::Convex *convex = new rn::Convex(shape.point_count());
	for (size_t i = 0; i < shape.point_count(); i++)
		convex->setPoint(i, shape.points[i]);
	return convex;
}