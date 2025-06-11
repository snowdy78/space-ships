#pragma once

#include "SpaceFieldObject.hpp"
#include "SpaceFieldStatement.hpp"

class SpaceItem : virtual GameObject, public SpaceFieldStatement
{
public:
	using OwnerPtrType = std::weak_ptr<SpaceFieldStatement>;
	void assignOwner(OwnerPtrType owner)
	{
		owner_ptr = std::move(owner);
	}
	template<class T = SpaceFieldStatement>
	std::shared_ptr<T> getOwner() const
	{
		return std::dynamic_pointer_cast<T>(owner_ptr.lock());
	}
	template<class T>
	bool hasSameOwner(const T *possible_owner) const noexcept
	{
		if (!hasOwner())
			return false;
		return owner_ptr.lock().get() == dynamic_cast<const SpaceFieldObject *>(possible_owner);
	}
	bool hasOwner() const
	{
		return !owner_ptr.expired();
	}
private:
	OwnerPtrType owner_ptr;
};

