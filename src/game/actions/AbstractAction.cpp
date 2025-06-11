#include "game/actions/AbstractAction.hpp"

AbstractAction::~AbstractAction() {}

bool AbstractAction::playable() const
{
	return true;
}
