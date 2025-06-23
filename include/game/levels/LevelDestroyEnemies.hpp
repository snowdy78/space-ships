#pragma once

#include "AbstractLevel.hpp"
#include "game/actions/ActionHandler.hpp"

class LevelDestroyEnemies : public AbstractLevel
{
public:
	LevelDestroyEnemies(
		SpaceField &field, const Difficulty &difficulty, size_t enemy_remaining
	);
	~LevelDestroyEnemies() override;

	size_t getDestroyEnemyCount() const;
	size_t getInitialEnemyCount() const;
	size_t getRemainingToSummon() const;
	void start() override;
	void update() override;
	void decrease_remaining();
	void onSummon(const SpaceField::StatePtrType &ptr) override;
	bool summonCondition() const override;
	bool nextLevelCondition() const override;
	std::string getHeader() const override;
	std::string getDescription() const override;

private:
	constexpr static TimeDigitType time_check_dead_objects{1000};
	std::shared_ptr<ActionHandler> m_destroy_enemy_handler{nullptr};
	rn::Stopwatch m_clock;
	size_t m_enemy_remaining_to_summon;
	size_t m_need_to_destroy;
	size_t m_initial_enemy_count;
};
