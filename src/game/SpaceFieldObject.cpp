#include "game/SpaceFieldObject.hpp"
#include "game/SpaceField.hpp"

SpaceFieldObject::SpaceFieldObject() = default;

void SpaceFieldObject::destroy()
{
    onDestroy();
    destroyFromField();
}
