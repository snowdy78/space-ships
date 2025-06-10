#pragma once

#include "LevelFactory.hpp"


class Level1 final : public AbstractLevel
{
public:
	Level1(SpaceField &field);
	void start() override;

	void update() override;
	~Level1() override;
	std::unique_ptr<AbstractLevelFactory> next() const override;
	size_t factory_id() const override;
	/**
	 * @brief 35% chance to summon asteroid instead of enemy ship
	 * @return 
	 */
	PoolEntities::ConstIterator nextSummon() const override;
};

struct Level1Factory : BasicLevelFactory<Level1Factory, Level1>
{
};
