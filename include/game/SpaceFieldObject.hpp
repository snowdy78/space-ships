#pragma once

#include "GameObject.hpp"
#include "SpaceFieldStatement.hpp"
#include "decl.hpp"

class SpaceFieldObject : public virtual GameObject,
						 public rn::LogicalObject,
						 public sf::Drawable,
						 public sf::Transformable,
						 public SpaceFieldStatement
{
	friend class SpaceField;

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

protected:
	virtual void onDestroy()
	{
	}
	virtual void onSummon() const
	{
	}

private:
	bool need_destroy = false;
};
