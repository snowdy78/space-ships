#include "game/levels/Level1.hpp"

#include "Helpers.hpp"
#include "game/GameManager.hpp"
#include "game/actions/AbstractSummonAction.hpp"
#include "game/actions/SummonItemAction.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"
#include "game/guns/Pistol.hpp"
#include "game/levels/Level2.hpp"
#include "game/ships/EnemyShip.hpp"


Level1::Level1(SpaceField &field)
	: LevelDestroyEnemies(field, Difficulty::Star1, *props::enemy_count)
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

void Level1::afterHeaderShow()
{
	summon(std::min(getRemainingToSummon(), static_cast<size_t>(5)));
	m_clock.start();
}

void Level1::start()
{
	LevelDestroyEnemies::start();
	setSummonPackSize(*props::summon_count);
	if (GameManager::exist())
		GameManager::session()->action_manager.emplaceToTop<SummonItemAction<SpaceItem>>(
			Pistol::identifier, [](SpaceItem &item) {
				if (GameManager::exist())
				{
					auto view = GameManager::session()->camera.getViewRect();
					view.height /= 2;
					item.setPosition(rn::math::centerPadding(view, rn::Vec2f{}));
				}
			}
		);
}

void Level1::update()
{
	LevelDestroyEnemies::update();
	if (GameManager::exist() && everyTime(m_clock, m_asteroid_summon_time))
	{
		m_clock.reset();
		randomlySummonAsteroidOutsideArea<SimpleAsteroid>(GameManager::session()->camera.getViewRect());
	}
}

std::unique_ptr<AbstractLevelFactory> Level1::next() const
{
	return std::make_unique<Level2Factory>();
}

size_t Level1::factoryId() const
{
	return Level1Factory::identifier;
}


bool Level1::summonCondition() const
{
	return LevelDestroyEnemies::summonCondition() && !m_clock.is_stopped()
		   && std::all_of(begin(), end(), [](const Entities::ValueType &value) {
			   return value.expired();
		   });
}

AbstractLevel::PoolEntities::ConstIterator Level1::nextSummon() const
{
	return rn::random::item(poolBegin(), poolEnd());
}