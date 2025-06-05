#pragma once

#include "AbstractLevel.hpp"


template<class Derived>
class BasicLevelFactory : public AbstractLevelFactory
{
public:
	static size_t identifier;
	BasicLevelFactory() = default;
	level_ptr create() const final;
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
	static level_ptr create(key_type key);

	template<class T>
	size_t push();
	template<class T>
	static level_ptr create();

private:
	LevelFactory() = default;
	container_type<key_type, value_type> generators;
};

template<class Derived>
size_t BasicLevelFactory<Derived>::identifier = LevelFactory::identify<Derived>();

template<class Derived>
AbstractLevelFactory::level_ptr BasicLevelFactory<Derived>::create() const
{
	return std::make_unique<Derived>();
}

template<class T>
size_t LevelFactory::identify()
{
	return instance().push<T>();
}

template<class T>
size_t LevelFactory::push()
{
	generators.insert({ generators.size(), std::make_unique<T>() });
	return generators.size() - 1;
}

template<class T>
LevelFactory::level_ptr LevelFactory::create()
{
	return instance().generators.at(T::identifier)->create();
}
