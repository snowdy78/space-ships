#pragma once

#include "decl.hpp"
#include "GameObject.hpp"

class SpaceFieldObject : public virtual GameObject
{
	friend class SpaceField;
	bool need_destroy = false;

protected:
	virtual void onDestroy()
	{
	}
	virtual void onSummon() const
	{
	}

public:
	SpaceFieldObject();
	~SpaceFieldObject() override;
	bool willBeDestroyed() const;
	virtual void summonCopy(SpaceField &field) const = 0;
	/**
	 * @brief proceed destroying from the field
	 *
	 */
	void destroy();
};
