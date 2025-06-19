#pragma once

#include "AbstractLevel.hpp"

class LevelDestroyEnemies : public AbstractLevel
{
public:
	LevelDestroyEnemies(
		SpaceField &field, const Difficulty &difficulty, size_t enemy_remaining
	);
	~LevelDestroyEnemies() override;

	size_t remaining() const;
	void afterHeaderShow() override;
	void update() override;
	void onSummon() override;
	bool nextLevelCondition() const override;
	std::string getHeader() const override;
	std::string getDescription() const override;

private:
	size_t m_enemy_remaining;
};
