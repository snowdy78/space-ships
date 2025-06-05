#pragma once

#include "LevelFactory.hpp"

class Level1 : public AbstractLevel
{
public:
	Level1();
	void start() override;

	void update() override;
};

struct Level1Factory : BasicLevelFactory<Level1Factory> {};
