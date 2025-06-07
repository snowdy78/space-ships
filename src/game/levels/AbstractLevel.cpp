#include "game/levels/AbstractLevel.hpp"

AbstractLevelFactory::~AbstractLevelFactory() = default;

AbstractLevel::AbstractLevel(SpaceField &field, const Difficulty &difficulty)
	: m_difficulty_type(difficulty),
	  m_field(field)
{
}

AbstractLevel::~AbstractLevel() = default;

AbstractLevel::Difficulty AbstractLevel::getDifficulty() const
{
	return m_difficulty_type;
}

rn::Json AbstractLevel::toJson() const
{
	return {
		{ "factory_id", factory_id()								 },
		{ "difficulty", static_cast<std::uint8_t>(getDifficulty()) }
	};
}

void AbstractLevel::clear()
{
	entities.clear();
}

AbstractLevel::PoolEntities::PoolEntities(const std::initializer_list<size_t> &lst)
	: m_pool(lst.begin(), lst.end())
{
}

AbstractLevel::PoolEntities::ConstIterator AbstractLevel::PoolEntities::begin() const
{
	return m_pool.begin();
}

AbstractLevel::PoolEntities::ConstIterator AbstractLevel::PoolEntities::end() const
{
	return m_pool.end();
}

AbstractLevel::Entities::Entities(SpaceField &field, const PoolEntities &pool)
	: m_field(field),
	  m_pool(pool)
{
}

void AbstractLevel::Entities::clear()
{
	for (auto it = begin(); it != end(); it = destroy(it))
	{
	}
}

auto AbstractLevel::Entities::destroy(Iterator it)
{
	if (it->expired())
		return m_entities.erase(it);

	const auto obj = it->lock();
	obj->destroy();
	return m_entities.erase(it);
}

auto AbstractLevel::Entities::cend() const
{
	return m_entities.cend();
}

auto AbstractLevel::Entities::end() const
{
	return m_entities.end();
}

auto AbstractLevel::Entities::end()
{
	return m_entities.end();
}

auto AbstractLevel::Entities::cbegin() const
{
	return m_entities.cbegin();
}

auto AbstractLevel::Entities::begin() const
{
	return m_entities.begin();
}

auto AbstractLevel::Entities::begin()
{
	return m_entities.begin();
}
