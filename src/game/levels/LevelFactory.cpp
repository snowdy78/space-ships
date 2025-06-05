#include "game/levels/LevelFactory.hpp"


LevelFactory &LevelFactory::instance()
{
	static LevelFactory factory;
	return factory;
}

LevelFactory::level_ptr LevelFactory::create(key_type key)
{
	return instance().generators.at(key)->create();
}
