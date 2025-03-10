#pragma once

#include "decl.hpp"
#include "GameObject.hpp"

class SpaceFieldObject : public GameObject
{
    friend class SpaceField;
protected:
    SpaceField *field;
	void setField(SpaceField *field);

public:
	SpaceFieldObject(SpaceField *field = nullptr);

};
