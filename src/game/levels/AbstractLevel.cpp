#include "game/levels/AbstractLevel.hpp"

#include "Helpers.hpp"

AbstractLevelFactory::~AbstractLevelFactory() = default;

AbstractLevel::AbstractLevel(SpaceField &field, const Difficulty &difficulty)
	: m_difficulty_type(difficulty),
	  m_field(field)
{
}

AbstractLevel::~AbstractLevel() = default;

AbstractLevel::Difficulty AbstractLevel::getDifficultyType() const
{
	return m_difficulty_type;
}

float AbstractLevel::getDifficultyFactor() const
{
	return m_difficulty_factor;
}

float AbstractLevel::getDifficulty() const
{
	return m_difficulty;
}

rn::Json AbstractLevel::toJson() const
{
	return {
		{ "factory_id", factory_id()								 },
		{ "difficulty", static_cast<std::uint8_t>(getDifficulty()) }
	};
}

AbstractLevel::PoolEntities::ConstIterator AbstractLevel::pool_begin() const
{
	return m_pool.begin();
}

AbstractLevel::PoolEntities::ConstIterator AbstractLevel::pool_end() const
{
	return m_pool.end();
}

AbstractLevel::Entities::ConstIterator AbstractLevel::begin() const
{
	return m_entities.begin();
}

AbstractLevel::Entities::Iterator AbstractLevel::begin()
{
	return m_entities.begin();
}

AbstractLevel::Entities::ConstIterator AbstractLevel::end() const
{
	return m_entities.end();
}

AbstractLevel::Entities::Iterator AbstractLevel::end()
{
	return m_entities.end();
}


void AbstractLevel::start()
{
	LogicalObject::start();
}

void AbstractLevel::update()
{
	if (summonCondition())
	{
		summon();
	}
}

void AbstractLevel::summon(size_t count)
{
	while (count-- != 0)
	{
		m_entities.summon(nextSummon());
		onSummon();
	}
}

void AbstractLevel::erase(const Entities::ConstIterator &it)
{
	m_entities.destroy(it);
}

void AbstractLevel::clear()
{
	m_entities.clear();
}

AbstractLevel::PoolEntities::ObjectPtr AbstractLevel::PoolEntities::create(const ConstIterator iterator) const
{
	if (iterator == end())
		throw std::out_of_range("(std::out_of_range): object not found in entities pool\n");
	std::unique_ptr<GameObject> object = GameObjectFactory::create(iterator->entity_id());
	if (auto t_ptr = dynamic_unique_cast<SpaceFieldObject>(std::move(object)))
	{
		iterator->assign(t_ptr.get());
		if (!iterator->init_possible())
			throw std::runtime_error("Impossible to initialize the object");
		iterator->init();
		return t_ptr;
	}
	return nullptr;
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

AbstractLevel::Entities::Iterator AbstractLevel::Entities::destroy(ConstIterator it)
{
	if (it == end())
		throw std::out_of_range("cannot destroy end of array");
	if (it->expired())
		return m_entities.erase(it);

	const auto obj = it->lock();
	obj->destroy();
	return m_entities.erase(it);
}

AbstractLevel::Entities::ConstIterator AbstractLevel::Entities::cend() const
{
	return m_entities.cend();
}

AbstractLevel::Entities::ConstIterator AbstractLevel::Entities::end() const
{
	return m_entities.end();
}

AbstractLevel::Entities::Iterator AbstractLevel::Entities::end()
{
	return m_entities.end();
}

AbstractLevel::Entities::ConstIterator AbstractLevel::Entities::cbegin() const
{
	return m_entities.cbegin();
}

AbstractLevel::Entities::ConstIterator AbstractLevel::Entities::begin() const
{
	return m_entities.begin();
}

AbstractLevel::Entities::Iterator AbstractLevel::Entities::begin()
{
	return m_entities.begin();
}

void AbstractLevel::Entities::summon(PoolEntities::ConstIterator it)
{
	auto object = m_pool.create(it);
	auto raw	= object.release();
	try
	{
		m_entities.push_back(m_field.push_back(raw));
	}
	catch (std::exception &err)
	{
		delete raw;
		throw err;
	}
}
