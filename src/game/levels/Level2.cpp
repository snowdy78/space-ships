#include "game/levels/Level2.hpp"
#include "Helpers.hpp"
#include "game/actions/SummonItemAction.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"
#include "game/guns/LaserRifle.hpp"
#include "game/ships/EnemyShip.hpp"
#include "game/asteroids/SmallAsteroid.hpp"


Level2::Level2(SpaceField &field)
	: LevelDestroyEnemies(field, Difficulty::Star2, *props::enemy_count)
{
	pool<EnemyShip, SimpleAsteroid>(
		[](EnemyShip &enemy) {
			if (GameManager::exist())
			{
				enemy.setTarget(GameManager::session()->player);
				auto &camera = GameManager::session()->camera;
				enemy.setPosition(
					randomPointOutsideArea(camera.getViewRect(), std::max(enemy.getSize().x, enemy.getSize().y))
				);
			}
		},
		[](SimpleAsteroid &asteroid) {
			if (GameManager::exist())
			{
				auto &camera = GameManager::session()->camera;
				randomBodyDirectionalOnAreaOutsideArea(camera.getViewRect(), asteroid, asteroid.getSize());
			}
		}
	);
}

void Level2::start()
{
	LevelDestroyEnemies::start();
	setSummonPackSize(*props::summon_count);
	if (GameManager::exist())
	{
		GameManager::session()->action_manager.emplaceToTop<SummonItemAction<SpaceItem>>(
			LaserRifle::identifier, [](SpaceItem &item) {
				if (!GameManager::exist())
					return;
				auto &camera = GameManager::session()->camera;
				item.setPosition(randomAreaPoint({ camera.getPosition(), camera.getViewSize() }));
			}
		);
	}
}

void Level2::afterHeaderShow()
{
	LevelDestroyEnemies::afterHeaderShow();
	summon(std::min(getRemainingToSummon(), static_cast<size_t>(*props::summon_count)));
}

bool Level2::summonCondition() const
{
	return LevelDestroyEnemies::summonCondition() && std::all_of(begin(), end(), [](const Entities::ValueType &value) {
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
