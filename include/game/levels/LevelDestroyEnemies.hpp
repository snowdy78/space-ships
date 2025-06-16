#pragma once

#include "AbstractLevel.hpp"

class LevelDestroyEnemies : public AbstractLevel
{
	rn::Stopwatch m_summon_clock;
	size_t m_enemy_remaining;

public:
	LevelDestroyEnemies(SpaceField &field, const Difficulty &difficulty, size_t enemy_remaining);
	~LevelDestroyEnemies() override;

	size_t remaining() const;
	void start() override;
	void update() override;
	void onSummon() override;
	bool nextLevelCondition() const override;
	/**
	 * @brief has some chance to summon asteroid instead of enemy ship
	 * @return
	 */
	std::string getDescription() const override;
};
