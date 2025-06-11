#include "game/SpaceFieldStatement.hpp"

bool SpaceFieldStatement::existOnField() const
{
	return !m_self.expired();
}

SpaceFieldStatement::SelfPointerType &SpaceFieldStatement::self()
{
	return m_self;
}

const SpaceFieldStatement::SelfPointerType &SpaceFieldStatement::self() const
{
	return m_self;
}
