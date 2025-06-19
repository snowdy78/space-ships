#include "game/levels/EmptyLevel.hpp"

#include "game/levels/Level1.hpp"

EmptyLevel::EmptyLevel(SpaceField &field)
	: AbstractLevel(field, Difficulty::Star5)
{

}

size_t EmptyLevel::factoryId() const
{
	return EmptyLevelFactory::identifier;
}

void EmptyLevel::onEvent(sf::Event &event)
{
	AbstractLevel::onEvent(event);
	if (rn::isKeydown(sf::Keyboard::N))
	{
		m_next_level = true;
	}
}

bool EmptyLevel::summonCondition() const
{
	return false;
}

bool EmptyLevel::nextLevelCondition() const
{
	return m_next_level;
}

AbstractLevel::PoolEntities::ConstIterator EmptyLevel::nextSummon() const
{
	return poolEnd();
}

std::unique_ptr<AbstractLevelFactory> EmptyLevel::next() const
{
	return std::make_unique<Level1Factory>();
}

std::string EmptyLevel::getDescription() const
{
	return "press N button to get next level";
}

