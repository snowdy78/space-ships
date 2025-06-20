#include "game/levels/LevelDestroyEnemies.hpp"
#include "Helpers.hpp"

LevelDestroyEnemies::LevelDestroyEnemies(SpaceField &field, const Difficulty &difficulty, size_t enemy_remaining)
	: AbstractLevel(field, difficulty),
	  m_enemy_remaining(enemy_remaining)
{
}

void LevelDestroyEnemies::afterHeaderShow()
{
	AbstractLevel::afterHeaderShow();
}

void LevelDestroyEnemies::update()
{
	AbstractLevel::update();
}

LevelDestroyEnemies::~LevelDestroyEnemies() = default;

size_t LevelDestroyEnemies::remaining() const
{
	return m_enemy_remaining;
}

void LevelDestroyEnemies::onSummon()
{
	if (m_enemy_remaining != 0)
		m_enemy_remaining--;
}

bool LevelDestroyEnemies::nextLevelCondition() const
{
	return m_enemy_remaining == 0;
}

std::string LevelDestroyEnemies::getHeader() const
{
	return "Level";
}

std::string LevelDestroyEnemies::getDescription() const
{
	return std::string("Destroy ") + std::to_string(remaining()) + "enemies";
}
