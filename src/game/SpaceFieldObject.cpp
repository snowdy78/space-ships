#include "game/SpaceFieldObject.hpp"
#include "game/SpaceField.hpp"

SpaceFieldObject::SpaceFieldObject() = default;

SpaceFieldObject::~SpaceFieldObject()
{
}

void SpaceFieldObject::destroy()
{
    onDestroy();
    doDestroy();
}
