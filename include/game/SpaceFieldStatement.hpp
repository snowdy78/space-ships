#pragma once

#include "decl.hpp"
#include "GameObject.hpp"

struct SpaceFieldStatement : virtual GameObject
{
	friend class SpaceField;
	using SelfPointerType = std::weak_ptr<SpaceFieldStatement>;
	bool existOnField() const;
	/**
	 * @return field state pointer on this object or nullptr (does not exist at constructor!)
	 */
	SelfPointerType &self();
	/**
	 * @return field state pointer on this object (does not exist at constructor!)
	 */
	const SelfPointerType &self() const;

private:
	SelfPointerType m_self{};
};

template<class T, class U>
std::weak_ptr<T> dynamic_state_ptr_cast(const std::weak_ptr<U> &ptr)
{
	if (!ptr.expired())
		return std::dynamic_pointer_cast<T>(ptr.lock());
	return {};
}
