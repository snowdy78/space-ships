#pragma once

#include "components/Jsonable.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/ActionManager.hpp"

class AbstractLevelFactory;
class AbstractLevel;

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

	explicit AbstractLevel(const Difficulty &difficulty);
	~AbstractLevel() override = 0;
	Difficulty getDifficulty() const;
	rn::Json toJson() const final;

	virtual std::unique_ptr<AbstractLevelFactory> next() const = 0;
	virtual size_t factory_id() const						   = 0;

	void clear()
	{
		for (auto entity_id : entities)
		{
		}
		entities.clear();
	}

protected:
	struct Entities : std::vector<size_t>
	{
		Entities(SpaceField *field = nullptr)
			: field(field)
		{
		}
		template<class T>
		void add()
		{
			this->emplace_back(T::identifier);
		}
	private:
		friend class AbstractLevel;
		SpaceField *field = nullptr;
	};

	/**
	 * @brief Any entity produced exists in pool of entities
	 * 
	 */
	Entities pool_entities;
	/**
	 * @brief all created entities by current level
	 */
	Entities entities;

private:
	const Difficulty m_difficulty;
	/**
	 * @brief dynamically created field, but when level become next - field is moving to next level
	 */
	std::unique_ptr<SpaceField> field = nullptr;
	/**
	 * @brief dynamically created action manager, but when level become next - manager is moving to next level
	 */
	std::unique_ptr<ActionManager> action_manager = nullptr;

	friend void level_become_next(std::unique_ptr<AbstractLevel> &level);
};

void level_become_next(std::unique_ptr<AbstractLevel> &level);

class AbstractLevelFactory
{
public:
	using level_ptr = std::unique_ptr<AbstractLevel>;
	virtual ~AbstractLevelFactory();
	virtual level_ptr create() const = 0;
};
