#include "game/levels/LevelDestroyEnemies.hpp"
#include "Helpers.hpp"
#include "game/actions/DestroySpaceFieldObjectAction.hpp"
#include "game/ships/EnemyShipBase.hpp"

LevelDestroyEnemies::LevelDestroyEnemies(SpaceField &field, const Difficulty &difficulty, size_t enemy_remaining)
	: AbstractLevel(field, difficulty),
	  m_enemy_remaining_to_summon(enemy_remaining),
	  m_need_to_destroy(0),
	  m_initial_enemy_count(enemy_remaining)
{
}


LevelDestroyEnemies::~LevelDestroyEnemies() = default;

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
	m_clock.start();
	if (GameManager::exist())
	{
		m_destroy_enemy_handler.reset(new ActionHandler(DestroySpaceFieldObjectAction::identifier, [this]() {
			m_need_to_destroy = std::count_if(begin(), end(), [](const Entities::ValueType &value) {
				return !(value.expired() || value.lock()->willBeDestroyed());
			});
			updateDescription();
		}));
		GameManager::session()->action_manager.hook(m_destroy_enemy_handler);
	}
}

void LevelDestroyEnemies::update()
{
	AbstractLevel::update();
	if (everyTime(m_clock, time_check_dead_objects))
	{
		m_clock.reset();
		m_need_to_destroy = std::count_if(begin(), end(), [](const Entities::ValueType &value) {
			return !(value.expired() || value.lock()->willBeDestroyed());
		});
		updateDescription();
	}
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

void LevelDestroyEnemies::onSummon(const SpaceField::StatePtrType &ptr)
{
	auto state = dynamic_state_ptr_cast<AbstractEnemyShip>(ptr);
	if (!state.expired())
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
