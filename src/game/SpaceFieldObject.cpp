#include "game/SpaceFieldObject.hpp"
#include "game/SpaceField.hpp"

SpaceFieldObject::SpaceFieldObject() = default;

SpaceFieldObject::~SpaceFieldObject() = default;

bool SpaceFieldObject::willBeDestroyed() const
{
	return need_destroy;
}

void SpaceFieldObject::destroy()
{
	if (!willBeDestroyed())
	{
		onDestroy();
		need_destroy = true;
	}
}
