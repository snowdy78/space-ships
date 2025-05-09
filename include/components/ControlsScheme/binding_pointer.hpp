#pragma once
#include "ControlsSchemeDecl.hpp"
/**
 * @brief binding_pointer
 * In ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...>
 *
 * @tparam BindingSepE
 * @tparam KeyIdT
 * @tparam AcceptedKeyTypes
 * @tparam PropsTypes
 */
template<BindingSeparatorConcept BindingSepE, class KeyIdT, class AcceptedKeyTypes, class... PropsTypes>
class ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...>::binding_pointer
{
	std::unique_ptr<binding> item{ nullptr };

public:
	binding_pointer(binding *&&item)
		: item(std::move(item))
	{
	}
	binding_pointer(const binding_pointer &p)
	{
		if (p.item)
			item.reset(new binding(*p.item));
	}
	const binding &operator*() const
	{
		return *item;
	}
	const binding *operator->() const
	{
		return item.get();
	}
	bool operator==(const binding_pointer &other) const
	{
		return *item == *other.item;
	}
	bool operator!=(const binding_pointer &other) const
	{
		return !operator==(other);
	}
	operator bool() const
	{
		return bool(item);
	}
	binding_pointer &operator=(const binding_pointer &p)
	{
		if (this != &p)
		{
			if (p.item)
				item.reset(new binding(*p.item));
		}
		return *this;
	}
};
