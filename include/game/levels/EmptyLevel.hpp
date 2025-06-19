#pragma once

#include "AbstractLevel.hpp"
#include "LevelFactory.hpp"
class EmptyLevel final : public AbstractLevel
{
	bool m_next_level = false;

public:
	EmptyLevel(SpaceField &field);
	size_t factoryId() const override;
	void onEvent(sf::Event &event) override;
	bool summonCondition() const override;
	bool nextLevelCondition() const override;
	PoolEntities::ConstIterator nextSummon() const override;
	std::unique_ptr<AbstractLevelFactory> next() const override;
	std::string getDescription() const override;
};

struct EmptyLevelFactory : BasicLevelFactory<EmptyLevelFactory, EmptyLevel>
{};