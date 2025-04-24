#include "game/SpaceFieldObject.hpp"
#include "game/SpaceField.hpp"

SpaceFieldObject::SpaceFieldObject(SpaceField *field)
	: field(field) {};


void SpaceFieldObject::setField(SpaceField *field)
{
	this->field = field;
}
