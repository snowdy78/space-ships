#include "game/levels/LevelDestroyEnemies.hpp"
#include "Helpers.hpp"
#include "game/actions/DestroySpaceFieldObjectAction.hpp"

LevelDestroyEnemies::LevelDestroyEnemies(SpaceField &field, const Difficulty &difficulty, size_t enemy_remaining)
	: AbstractLevel(field, difficulty),
	  m_enemy_remaining_to_summon(enemy_remaining),
	  m_need_to_destroy(0),
	  m_initial_enemy_count(enemy_remaining)
{
}


LevelDestroyEnemies::~LevelDestroyEnemies()
{
	if (GameManager::exist())
		GameManager::session()->field.removeDestroyHandler(m_destroy_enemy_handler);
}

size_t LevelDestroyEnemies::getDestroyEnemyCount() const
{
	return m_need_to_destroy;
}

size_t LevelDestroyEnemies::getInitialEnemyCount() const
{
	return m_initial_enemy_count;
}

size_t LevelDestroyEnemies::getRemainingToSummon() const
{
	return m_enemy_remaining_to_summon;
}

void LevelDestroyEnemies::start()
{
	AbstractLevel::start();
	if (GameManager::exist())
		GameManager::session()->field.appendDestroyHandler([this](const SpaceField::StatePtrType &value) {
			if (value.expired())
				return;
			auto i = std::any_of(begin(), end(), [&value](const Entities::ValueType &ptr) {
				return !ptr.expired() && ptr.lock() == value.lock();
			});
			if (i)
			{
				std::cout << "destroy\n";
				m_need_to_destroy--;
				updateDescription();
			}

		});
}

void LevelDestroyEnemies::decrease_remaining()
{
	if (m_enemy_remaining_to_summon != 0)
	{
		m_enemy_remaining_to_summon--;
		m_need_to_destroy++;
		updateDescription();
	}
}

void LevelDestroyEnemies::onSummon()
{
	decrease_remaining();
}

bool LevelDestroyEnemies::summonCondition() const
{
	return m_enemy_remaining_to_summon != 0;
}

bool LevelDestroyEnemies::nextLevelCondition() const
{
	return getDestroyEnemyCount() == 0 && getRemainingToSummon() == 0
		   && std::all_of(begin(), end(), [](const Entities::ValueType &value) {
				  return value.expired();
			  });
}

std::string LevelDestroyEnemies::getHeader() const
{
	return "Level";
}

std::string LevelDestroyEnemies::getDescription() const
{
	return std::string("Destroy ") + std::to_string(getDestroyEnemyCount()) + "/"
		   + std::to_string(getInitialEnemyCount()) + " enemies.";
}
