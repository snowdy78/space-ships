#include "game/asteroids/SimpleAsteroid.hpp"
#include "game/SpaceField.hpp"

SimpleAsteroid::SimpleAsteroid(): AbstractAsteroid(*texture)
{
}

void SimpleAsteroid::summonCopy(SpaceField &field) const
{
	field.summonAsteroid<SimpleAsteroid>([this](SimpleAsteroid &asteroid) {
		asteroid.setPosition(getPosition());
		asteroid.setDirection(getDirection());
		asteroid.setVelocity(getVelocity());
		asteroid.setMass(getMass());
		asteroid.setAcceleration(getAcceleration());
	});
}

void SimpleAsteroid::updateCollider()
{
	rn::math::rectangle rect{ getGlobalBounds() };
	collider.transform(rect);
}
