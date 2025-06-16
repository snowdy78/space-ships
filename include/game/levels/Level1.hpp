#pragma once

#include "LevelFactory.hpp"
#include "components/GameConfiguration.hpp"
#include "Font.hpp"

class Level1 final : public AbstractLevel
{
	struct props
	{
		inline static auto &self_config = config::instance().get("Level1");
		G_CONFIG_PROP_DEFINE(self_config, summon_chance);
		G_CONFIG_PROP_DEFINE(self_config, summon_time);
		G_CONFIG_PROP_DEFINE(self_config, enemy_count);
	};
	rn::Stopwatch summon_clock;
	sf::Text description{ "", *Font::Default };
	size_t enemy_remaining = *props::enemy_count;

public:
	Level1(SpaceField &field);
	~Level1() override;
	
	void start() override;
	void update() override;
	std::unique_ptr<AbstractLevelFactory> next() const override;
	size_t factoryId() const override;
	void onSummon() override;
	bool nextLevelCondition() const override;
	bool summonCondition() const override;
	/**
	 * @brief has some chance to summon asteroid instead of enemy ship
	 * @return 
	 */
	PoolEntities::ConstIterator nextSummon() const override;
	std::string getDescription() const override;
};

struct Level1Factory : BasicLevelFactory<Level1Factory, Level1>
{
};
