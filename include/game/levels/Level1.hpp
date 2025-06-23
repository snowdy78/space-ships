#pragma once

#include "LevelFactory.hpp"
#include "components/GameConfiguration.hpp"
#include "LevelDestroyEnemies.hpp"

class Level1 final : public LevelDestroyEnemies
{
	struct props
	{
		inline static auto &self_config = config::instance().get("Level1");
		G_CONFIG_PROP_DEFINE(self_config, summon_count);
		G_CONFIG_PROP_DEFINE(self_config, asteroid_summon_time);
		G_CONFIG_PROP_DEFINE(self_config, enemy_count);
	};
	rn::Stopwatch m_clock;
	TimeDigitType m_asteroid_summon_time{*props::asteroid_summon_time};

public:
	Level1(SpaceField &field);

	void afterHeaderShow() override;
	void start() override;
	void update() override;
	std::unique_ptr<AbstractLevelFactory> next() const override;
	size_t factoryId() const override;
	bool summonCondition() const override;
	/**
	 * @brief has some chance to summon asteroid instead of enemy ship
	 * @return 
	 */
	PoolEntities::ConstIterator nextSummon() const override;
};

struct Level1Factory : BasicLevelFactory<Level1Factory, Level1>
{
};
