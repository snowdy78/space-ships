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
	pool<EnemyShip>(
		[](EnemyShip &enemy) {
			if (GameManager::exist())
			{
				enemy.setTarget(GameManager::session()->player);
				auto &camera = GameManager::session()->camera;
				enemy.setPosition(
					randomPointOutsideArea(camera.getViewRect(), std::max(enemy.getSize().x, enemy.getSize().y))
				);
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

void Level2::update()
{
	LevelDestroyEnemies::update();
	if (GameManager::exist() && everyTime(m_clock, m_asteroid_summon_time))
	{
		m_clock.reset();
		randomlySummonAsteroidOutsideArea<SimpleAsteroid>(GameManager::session()->camera.getViewRect());
	}
}

void Level2::afterHeaderShow()
{
	LevelDestroyEnemies::afterHeaderShow();
	summon(std::min(getRemainingToSummon(), static_cast<size_t>(*props::summon_count)));
	m_clock.start();
}

bool Level2::summonCondition() const
{
	return LevelDestroyEnemies::summonCondition() && !m_clock.is_stopped()
		   && std::all_of(begin(), end(), [](const Entities::ValueType &value) {
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
