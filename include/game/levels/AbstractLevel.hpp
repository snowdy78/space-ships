#pragma once

#include "components/Jsonable.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/ActionManager.hpp"

class AbstractLevelFactory;
class AbstractLevel;

template<class T>
concept IdentifiedSpaceObjectConcept = std::is_base_of_v<SpaceFieldObject, T> && std::is_final_v<T> && requires() { T::identifier; };

class AbstractLevel : public Jsonable, public rn::LogicalObject
{
public:
	enum class Difficulty
	{
		Star1 = 1,
		Star2,
		Star3,
		Star4,
		Star5
	};

	explicit AbstractLevel(SpaceField &field, const Difficulty &difficulty);
	~AbstractLevel() override = 0;
	Difficulty getDifficulty() const;
	rn::Json toJson() const final;

	virtual std::unique_ptr<AbstractLevelFactory> next() const = 0;
	virtual size_t factory_id() const						   = 0;

	void clear();

private:
	struct PoolEntities
	{
		template<class T>
		using Container				= std::vector<T>;
		using PoolEntitiesContainer = Container<size_t>;
		using Iterator				= PoolEntitiesContainer::iterator;
		using ConstIterator			= PoolEntitiesContainer::const_iterator;
		PoolEntities()				= default;
		PoolEntities(const std::initializer_list<size_t> &lst);

		ConstIterator begin() const;
		ConstIterator end() const;

		template<SpaceFieldObjectConcept T>
		void push();
		template<SpaceFieldObjectConcept T>
		ConstIterator find() const;
		template<class T>
		T *create()

	private:
		PoolEntitiesContainer m_pool;
	};
	struct Entities
	{
		template<class T>
		using Container			= std::vector<T>;
		using EntitiesContainer = Container<SpaceField::StatePtrType>;
		using Iterator			= EntitiesContainer::iterator;
		using ConstIterator		= EntitiesContainer::const_iterator;
		Entities(SpaceField &field);

		Iterator begin();
		ConstIterator begin() const;
		ConstIterator cbegin() const;
		Iterator end();
		ConstIterator end() const;
		ConstIterator cend() const;

		Iterator destroy(Iterator it);
		void summon(PoolEntities::ConstIterator);
		void clear();

	private:
		friend class AbstractLevel;
		SpaceField &field;
		EntitiesContainer m_entities{};
	};
	
	const Difficulty m_difficulty;
	/**
	 * @brief pointer on field
	 */
	SpaceField &m_field;
	/**
	 * @brief all created entities by current level
	 */
	Entities entities{ m_field };

protected:
	PoolEntities pool_entities;

};

template<SpaceFieldObjectConcept T>
void AbstractLevel::PoolEntities::push()
{
	m_pool.push_back(T::identifier);
}

template<SpaceFieldObjectConcept T>
AbstractLevel::PoolEntities::ConstIterator AbstractLevel::PoolEntities::find() const
{
	return std::find(begin(), end(), T::identifier);
}

class AbstractLevelFactory
{
public:
	using level_ptr = std::unique_ptr<AbstractLevel>;
	virtual ~AbstractLevelFactory();
	virtual level_ptr create() const = 0;
};
