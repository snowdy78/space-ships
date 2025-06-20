#include "game/actions/AbstractAction.hpp"

AbstractAction::~AbstractAction() {}

bool AbstractAction::playable() const
{
	return true;
}
#ifdef SPACE_SHIP_DEBUG
const std::string & AbstractAction::getName()
{
	return name;
}
#endif