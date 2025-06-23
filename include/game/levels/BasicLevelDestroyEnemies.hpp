#pragma once

#include "LevelDestroyEnemies.hpp"
#include "LevelFactory.hpp"
#include "components/GameConfiguration.hpp"
#include "game/actions/SummonItemAction.hpp"
#include "game/ships/EnemyShipBase.hpp"

template<
	class Derived, class NextLevelFactory, const AbstractLevel::Difficulty Diff, char const *ClassName,
	AsteroidConcept... AsteroidsTypes>
class BasicLevelDestroyEnemies : public LevelDestroyEnemies
{
	struct props
	{
		inline static auto &self_config = config::instance().get(ClassName);
		G_CONFIG_PROP_DEFINE(self_config, summon_count);
		G_CONFIG_PROP_DEFINE(self_config, asteroid_summon_time);
		G_CONFIG_PROP_DEFINE(self_config, enemy_count);
	};
	struct LevelFactory final : BasicLevelFactory<LevelFactory, Derived>
	{
	};
protected:
	template<WeaponConcept Weapon>
	static void randomlySummonWeapon()
	{
		if (GameManager::exist())
			GameManager::session()->action_manager.emplaceToTop<SummonItemAction<SpaceItem>>(
				Weapon::identifier, [](SpaceItem &item) {
					if (!GameManager::exist())
						return;
					auto &camera = GameManager::session()->camera;
					item.setPosition(randomAreaPoint(camera.getViewRect()));
				}
			);
	}
	static void randomlySummonEnemy(AbstractEnemyShip &enemy)
	{
		if (GameManager::exist())
		{
			enemy.setTarget(GameManager::session()->player);
			auto &camera = GameManager::session()->camera;
			enemy.setPosition(
				randomPointOutsideArea(camera.getViewRect(), std::max(enemy.getSize().x, enemy.getSize().y))
			);
		}
	}

public:
	BasicLevelDestroyEnemies(SpaceField &field)
		: LevelDestroyEnemies(field, Diff, *props::enemy_count)
	{
		this->setSummonPackSize(*props::summon_count);
		setAsteroidSummonTime(TimeDigitType{ *props::asteroid_summon_time });
	}

	size_t factoryId() const override
	{
		return LevelFactory::identifier;
	}
	std::unique_ptr<AbstractLevelFactory> next() const override
	{
		return std::make_unique<NextLevelFactory>();
	}
	void onAsteroidSummonTiming() override
	{
		randomlySummonAsteroidOutsideArea<AsteroidsTypes...>(GameManager::session()->camera.getViewRect());
	}
};
