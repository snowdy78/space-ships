#pragma once

#include "LevelDestroyEnemies.hpp"
#include "LevelFactory.hpp"
#include "components/GameConfiguration.hpp"

class Level2 final : public LevelDestroyEnemies
{
	struct props
	{
		inline static auto &self_config = config::instance().get("Level2");
		G_CONFIG_PROP_DEFINE(self_config, summon_count);
		G_CONFIG_PROP_DEFINE(self_config, enemy_count);
	};

public:
	Level2(SpaceField &field);

	void start() override;
	void afterHeaderShow() override;
	size_t factoryId() const override;
	std::unique_ptr<AbstractLevelFactory> next() const override;
	bool summonCondition() const override;
	PoolEntities::ConstIterator nextSummon() const override;
};

struct Level2Factory final : BasicLevelFactory<Level2Factory, Level2>
{
};
