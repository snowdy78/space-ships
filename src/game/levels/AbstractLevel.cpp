#include "game/levels/AbstractLevel.hpp"

AbstractLevelFactory::~AbstractLevelFactory() = default;

AbstractLevel::AbstractLevel(const Difficulty &difficulty)
	: m_difficulty()
{
}

AbstractLevel::~AbstractLevel() = default;

AbstractLevel::Difficulty AbstractLevel::getDifficulty() const
{
	return m_difficulty;
}

rn::Json AbstractLevel::toJson() const
{
	return
	{
		{ "factory_id", factory_id() },
		{ "difficulty", static_cast<std::uint8_t>(getDifficulty()) }
	};
}

void level_become_next(std::unique_ptr<AbstractLevel> &level)
{
	if (!level)
		throw std::runtime_error("cannot move nullptr to next level");
	auto next_factory = level->next();
	auto next_level	  = next_factory->create();
	next_level->field.swap(level->field);
	next_level->action_manager.swap(level->action_manager);
	level.swap(next_level);
}
