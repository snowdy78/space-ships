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

    virtual void summonCopy(SpaceField *field) const = 0;
    /**
     * @brief destroying from the field 
     * 
     */
    virtual void destroyFromField() const = 0;
    virtual void onSummon() const {}
};
