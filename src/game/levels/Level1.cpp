#include "game/levels/Level1.hpp"

#include "Helpers.hpp"
#include "game/EnemyShip.hpp"
#include "game/GameManager.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"

Level1::Level1(SpaceField &field)
	: AbstractLevel(field, Difficulty::Star1)
{
	pool<EnemyShip, SimpleAsteroid>(
		[](EnemyShip &enemy) {
			if (GameManager::exist())
			{
				enemy.setTarget(GameManager::session()->player);
				enemy.setPosition(
					randomPointOutsideArea(
						GameManager::session()->camera.getView().getViewport(),
						std::max(enemy.getSize().x, enemy.getSize().y)
					)
				);
			}
		},
		[](SimpleAsteroid &asteroid) {
			randomBodyDirectionalOnAreaOutsideArea(
				GameManager::session()->camera.getView().getViewport(), asteroid, *SimpleAsteroid::velocity,
				asteroid.getSize()
			);
		}
	);
}

void Level1::start()
{
	AbstractLevel::start();
}

void Level1::update()
{
	AbstractLevel::update();
}

Level1::~Level1() = default;

std::unique_ptr<AbstractLevelFactory> Level1::next() const
{
	return std::make_unique<Level1Factory>();
}

size_t Level1::factory_id() const
{
	return Level1Factory::identifier;
}

AbstractLevel::PoolEntities::ConstIterator Level1::nextSummon() const
{
	if (rn::random::chance(0.35))
		return pool_find<SimpleAsteroid>();
	return pool_find<EnemyShip>();
}

