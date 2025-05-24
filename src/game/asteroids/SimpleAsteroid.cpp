#include "game/asteroids/SimpleAsteroid.hpp"

SimpleAsteroid::SimpleAsteroid(): AbstractAsteroid(*texture)
{
}

void SimpleAsteroid::summonCopy(SpaceField &field) const
{
	field.summonAsteroid<SimpleAsteroid>(getPosition(), countMove());
}

void SimpleAsteroid::updateCollider()
{
	rn::math::rectangle rect{ getGlobalBounds() };
	collider.transform(rect);
}
