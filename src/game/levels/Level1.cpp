#include "game/levels/Level1.hpp"

#include "game/EnemyShip.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"

Level1::Level1(SpaceField &field)
	: AbstractLevel(field, Difficulty::Star1)
{
	pool_entities.assign<EnemyShip, SimpleAsteroid>();
}

void Level1::start()
{
	AbstractLevel::start();
}

void Level1::update()
{
	AbstractLevel::update();
}
