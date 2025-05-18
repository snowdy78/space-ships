#pragma once

#include "decl.hpp"
#include "GameObject.hpp"

class SpaceFieldObject : public virtual GameObject
{
	friend class SpaceField;

protected:
	virtual void onDestroy()
	{
	}
	virtual void onSummon() const
	{
	}
	virtual void destroyFromField() = 0;

public:
	SpaceFieldObject();
	~SpaceFieldObject() override;
	virtual void summonCopy(SpaceField &field) const = 0;
	/**
	 * @brief proceed destroying from the field
	 *
	 */
	void destroy();
};
