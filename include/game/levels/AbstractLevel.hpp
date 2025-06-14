#pragma once

#include "components/Jsonable.hpp"
#include "game/SpaceField.hpp"

class AbstractLevelFactory;
class AbstractLevel;

template<class T>
concept FinalSpaceObjectConcept
	= std::is_base_of_v<SpaceFieldObject, T> && std::is_final_v<T> && requires() { T::identifier; };

class AbstractLevel : public Jsonable, public rn::LogicalObject
{
public:
	struct PoolEntities
	{
		template<class T>
		using InitializerFunc	= std::function<void(T &)>;
		using BaseObject		= SpaceFieldObject;
		using InitializerObject = InitializerFunc<BaseObject>;
		template<class T>
		using SmartPtr	= std::unique_ptr<T>;
		using ObjectPtr = SmartPtr<BaseObject>;
		template<class T>
		using Container = std::vector<T>;
		class AbstractInitializer
		{
		public:
			virtual ~AbstractInitializer()	  = default;
			virtual bool possible() const	  = 0;
			virtual void assign(BaseObject *) = 0;
			virtual void initialize() const	  = 0;
		};
		template<class T>
		class BasicInitializer : public AbstractInitializer
		{
		public:
			BasicInitializer(InitializerFunc<T> init_func)
				: m_init_base(nullptr),
				  m_init(std::move(init_func))
			{
			}
			bool possible() const override
			{
				return m_init_base;
			}
			void assign(BaseObject *init_object) override
			{
				m_init_base = init_object;
			}
			void initialize() const override
			{
				if (T *init_object = dynamic_cast<T *>(m_init_base))
					m_init(*init_object);
			}

		private:
			BaseObject *m_init_base = nullptr;
			InitializerFunc<T> m_init;
		};
		class PoolItem
		{
		public:
			template<class T>
			PoolItem(size_t object_id, InitializerFunc<T> init)
				: m_entity_id(object_id),
				  m_init(new BasicInitializer<T>(std::move(init)))
			{
			}
			void assign(BaseObject *obj) const
			{
				m_init->assign(obj);
			}
			size_t entity_id() const
			{
				return m_entity_id;
			}
			bool init_possible() const
			{
				return m_init->possible();
			}
			void init() const
			{
				m_init->initialize();
			}

		private:
			size_t m_entity_id;
			SmartPtr<AbstractInitializer> m_init;
		};
		using PoolEntitiesContainer = Container<PoolItem>;
		using Iterator				= PoolEntitiesContainer::iterator;
		using ConstIterator			= PoolEntitiesContainer::const_iterator;
		PoolEntities()				= default;

		ConstIterator begin() const;
		ConstIterator end() const;
		template<FinalSpaceObjectConcept... Types>
		void assign(InitializerFunc<Types>... initializers);
		template<FinalSpaceObjectConcept T>
		void push(const InitializerFunc<T> &init_func);
		template<FinalSpaceObjectConcept T>
		ConstIterator find() const;

		template<FinalSpaceObjectConcept T>
		bool contains() const;
		/**
		 * @brief factory function. Finds object in pool, next game object factory and returns nullptr if not found,
		 * T pointer otherwise
		 * @tparam T - finding object pointer
		 * @param iterator - pool iterator
		 * @throws std::out_of_range
		 * @throws std::bad_cast
		 * @return nullptr or T *
		 */
		ObjectPtr create(ConstIterator iterator) const;

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
		Entities(SpaceField &field, const PoolEntities &pool);

		Iterator begin();
		ConstIterator begin() const;
		ConstIterator cbegin() const;
		Iterator end();
		ConstIterator end() const;
		ConstIterator cend() const;

		Iterator destroy(ConstIterator it);
		void summon(PoolEntities::ConstIterator it);
		void clear();

	private:
		friend class AbstractLevel;
		SpaceField &m_field;
		const PoolEntities &m_pool;
		EntitiesContainer m_entities{};
	};
	using TimeDigitType = std::chrono::milliseconds;
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
	rn::Json toJson() const final;
	PoolEntities::ConstIterator pool_begin() const;
	PoolEntities::ConstIterator pool_end() const;
	Entities::ConstIterator begin() const;
	Entities::Iterator begin();
	Entities::ConstIterator end() const;
	Entities::Iterator end();
	void start() override;
	void update() override;
	Difficulty getDifficultyType() const;
	float getDifficulty() const;
	float getDifficultyFactor() const;
	virtual void onSummon()
	{
	}
	virtual bool nextLevelCondition() const					   = 0;
	virtual bool summonCondition() const					   = 0;
	virtual std::string getDescription() const				   = 0;
	virtual std::unique_ptr<AbstractLevelFactory> next() const = 0;
	virtual size_t factory_id() const						   = 0;
	virtual PoolEntities::ConstIterator nextSummon() const	   = 0;
	template<FinalSpaceObjectConcept... Types>
	void pool(PoolEntities::InitializerFunc<Types>... initializers);
	template<FinalSpaceObjectConcept T>
	PoolEntities::ConstIterator pool_find() const;
	void summon(size_t count = 1);
	void erase(const Entities::ConstIterator &it);
	void clear();

private:
	const Difficulty m_difficulty_type;
	const float m_difficulty_factor = 1.f;
	const float m_difficulty		= m_difficulty_factor * static_cast<float>(m_difficulty_type);
	/**
	 * @brief field reference
	 */
	SpaceField &m_field;

	/**
	 * @brief pool of entities that can be created
	 */
	PoolEntities m_pool;

	/**
	 * @brief all created entities by level
	 */
	Entities m_entities{ m_field, m_pool };
};

template<FinalSpaceObjectConcept... Types>
void AbstractLevel::PoolEntities::assign(InitializerFunc<Types>... initializers)
{
	PoolItem init[] = { PoolItem(Types::identifier, std::move(initializers))... };
	m_pool
		= PoolEntitiesContainer{ std::make_move_iterator(std::begin(init)), std::make_move_iterator(std::end(init)) };
}

template<FinalSpaceObjectConcept T>
void AbstractLevel::PoolEntities::push(const InitializerFunc<T> &init_func)
{
	m_pool.emplace_back(new PoolItem(T::identifier, init_func));
}

template<FinalSpaceObjectConcept T>
AbstractLevel::PoolEntities::ConstIterator AbstractLevel::PoolEntities::find() const
{
	return std::find_if(begin(), end(), [](const PoolEntitiesContainer::value_type &item) {
		return item.entity_id() == T::identifier;
	});
}

template<FinalSpaceObjectConcept T>
bool AbstractLevel::PoolEntities::contains() const
{
	return find<T>() != end();
}

template<FinalSpaceObjectConcept... Types>
void AbstractLevel::pool(PoolEntities::InitializerFunc<Types>... initializers)
{
	m_pool.assign<Types...>(initializers...);
}

template<FinalSpaceObjectConcept T>
AbstractLevel::PoolEntities::ConstIterator AbstractLevel::pool_find() const
{
	return m_pool.find<T>();
}

class AbstractLevelFactory
{
public:
	using level_ptr = std::unique_ptr<AbstractLevel>;
	virtual ~AbstractLevelFactory();
	virtual level_ptr create(SpaceField &field) const = 0;
};
