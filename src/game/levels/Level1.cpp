#include "game/levels/Level1.hpp"

#include "Helpers.hpp"
#include "game/GameManager.hpp"
#include "game/actions/SummonItemAction.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"
#include "game/guns/Pistol.hpp"
#include "game/levels/Level2.hpp"
#include "game/ships/EnemyShip.hpp"

Level1::Level1(SpaceField &field)
	: LevelDestroyEnemies(field, Difficulty::Star1, *props::enemy_count)
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

void Level1::afterHeaderShow()
{
	summon(std::min(remaining(), static_cast<size_t>(5)));
	m_summon_clock.start();
}

void Level1::start()
{
	LevelDestroyEnemies::start();
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
}

std::unique_ptr<AbstractLevelFactory> Level1::next() const
{
	return std::make_unique<Level2Factory>();
}

size_t Level1::factoryId() const
{
	return Level1Factory::identifier;
}

void Level1::onSummon()
{
	LevelDestroyEnemies::onSummon();
	m_summon_clock.reset();
}

bool Level1::summonCondition() const
{
	return !nextLevelCondition() && everyTime(m_summon_clock, std::chrono::milliseconds(*props::summon_time));
}

AbstractLevel::PoolEntities::ConstIterator Level1::nextSummon() const
{
	if (rn::random::chance(*props::summon_chance))
		return poolFind<SimpleAsteroid>();
	return poolFind<EnemyShip>();
}

std::string Level1::getHeader() const
{
	return "Level ";
}
