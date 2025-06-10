#pragma once

#include "AbstractLevel.hpp"


template<class Derived, class CreateType>
class BasicLevelFactory : public AbstractLevelFactory
{
public:
	static size_t identifier;
	BasicLevelFactory() = default;
	level_ptr create(SpaceField &field) const final;
};

class LevelFactory
{
public:
	template<class Ke, class Va>
	using container_type   = std::unordered_map<Ke, Va>;
	using level_ptr		   = std::unique_ptr<AbstractLevel>;
	using create_func_type = std::unique_ptr<AbstractLevelFactory>;
	using key_type		   = size_t;
	using value_type	   = create_func_type;

	template<class T>
	static size_t identify();
	static LevelFactory &instance();
	static level_ptr create(key_type key, SpaceField &space);

	template<class T>
	size_t push();
	template<class T>
	static level_ptr create();

private:
	LevelFactory() = default;
	container_type<key_type, value_type> generators;
};

template<class Derived, class CreateType>
size_t BasicLevelFactory<Derived, CreateType>::identifier = LevelFactory::identify<Derived>();

template<class Derived, class CreateType>
AbstractLevelFactory::level_ptr BasicLevelFactory<Derived, CreateType>::create(SpaceField &field) const
{
	return std::make_unique<CreateType>(field);
}

template<class T>
size_t LevelFactory::identify()
{
	return instance().push<T>();
}

template<class T>
size_t LevelFactory::push()
{
	generators.emplace(generators.size(), std::move(std::make_unique<T>()));
	return generators.size() - 1;
}

template<class T>
LevelFactory::level_ptr LevelFactory::create()
{
	return instance().generators.at(T::identifier)->create();
}
