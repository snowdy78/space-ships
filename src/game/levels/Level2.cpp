#include "game/levels/Level2.hpp"
#include "game/EnemyShip.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"
#include "Helpers.hpp"

Level2::Level2(SpaceField &field)
	: LevelDestroyEnemies(field, Difficulty::Star2, *props::enemy_count)
{
	pool<EnemyShip, SimpleAsteroid>(
		[](EnemyShip &enemy) {
			if (GameManager::exist())
			{
				enemy.setTarget(GameManager::session()->player);
				auto &camera = GameManager::session()->camera;
				enemy.setPosition(randomPointOutsideArea(
					{ camera.getPosition(), camera.getViewSize() }, std::max(enemy.getSize().x, enemy.getSize().y)
				));
			}
		},
		[](SimpleAsteroid &asteroid) {
			if (GameManager::exist())
			{
				auto &camera = GameManager::session()->camera;
				randomBodyDirectionalOnAreaOutsideArea(
					{ camera.getPosition(), camera.getViewSize() }, asteroid, *SimpleAsteroid::velocity,
					asteroid.getSize()
				);
			}
		}
	);
}

void Level2::afterHeaderShow()
{
	LevelDestroyEnemies::afterHeaderShow();
	setSummonPackSize(*props::summon_count);
	summon(std::min(remaining(), static_cast<size_t>(*props::summon_count)));
}

bool Level2::summonCondition() const
{
	return std::all_of(begin(), end(), [](const Entities::ValueType &value) {
		return value.expired();
	});
}

size_t Level2::factoryId() const
{
	return Level2Factory::identifier;
}

std::unique_ptr<AbstractLevelFactory> Level2::next() const
{
	return std::make_unique<Level2Factory>();
}

AbstractLevel::PoolEntities::ConstIterator Level2::nextSummon() const
{
	return rn::random::item(poolBegin(), poolEnd());
}
