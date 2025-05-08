#pragma once

#include "decl.hpp"
#include "GameObject.hpp"

struct summon_traits
{
    rn::Vec2f position;
public:
    summon_traits() = default;
};

class SpaceFieldObject : public virtual GameObject
{
    friend class SpaceField;
protected:
    virtual void onDestroy() {}
    virtual void destroyFromField() = 0;
public:
	SpaceFieldObject();

    virtual void summonCopy(SpaceField *field) const = 0;
    /**
     * @brief destroying from the field 
     * 
     */
    void destroy();
    virtual void onSummon() const {}
};
