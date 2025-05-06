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
sf::FloatRect PolygonCollider::getBounds() const 
{
	sf::ConvexShape polygon{shape.point_count()};
	for (size_t i = 0; i < polygon.getPointCount(); i++)
	{
		polygon.setPoint(i, shape.point(i));
	}
	return polygon.getGlobalBounds();
}
const rn::math::shape &PolygonCollider::getColliderShape() const
{
	return shape;
}