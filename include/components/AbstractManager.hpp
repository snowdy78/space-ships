#pragma once

#include <memory>
#include <type_traits>
#include "decl.hpp"

template<template<class T> class ContainerT, class Ty>
class AbstractManager : public rn::LogicalObject
{
public:
	using value_type	   = Ty;
	using values_container = ContainerT<std::unique_ptr<Ty>>;
	using const_iterator   = values_container::const_iterator;
	using iterator		   = values_container::iterator;
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	iterator begin();
	iterator end();


	AbstractManager()			= default;
	~AbstractManager() override = 0;

	constexpr void push_back(const value_type &value);
	constexpr void push_back(value_type &&value) noexcept;

	template<class T>
	using after_emplace_func = std::function<void(T value)>;
	template<class T, class... Args>
	constexpr void emplace_back(const after_emplace_func<T *> &after_emplace, Args &&...args);
	template<class T, class... Args>
	constexpr void emplace_back(const after_emplace_func<T *> &after_emplace, const Args &...args);
	
	iterator erase(const_iterator index);
	size_t size() const;
	void clear();

	void update() override;
	/**
	 * @brief erase manager value predicate function
	 *
	 * @param item
	 * @return boolean
	 */
	virtual bool need_erase(const const_iterator &item) const = 0;
	/**
	 * @brief item update function
	 *
	 * @param item
	 */
	virtual void update_item(const const_iterator &item) = 0;

protected:
	/**
	 * @brief push value event handler
	 *
	 * @param value
	 */
	virtual void onPushValue(const const_iterator &value)
	{
	}

private:
	values_container m_container;
};

template<template<class T> class ContainerT, class Ty>
AbstractManager<ContainerT, Ty>::~AbstractManager()
{
}

template<template<class T> class ContainerT, class Ty>
typename AbstractManager<ContainerT, Ty>::const_iterator AbstractManager<ContainerT, Ty>::begin() const
{
	return m_container.begin();
}

template<template<class T> class ContainerT, class Ty>
typename AbstractManager<ContainerT, Ty>::const_iterator AbstractManager<ContainerT, Ty>::end() const
{
	return m_container.end();
}

template<template<class T> class ContainerT, class Ty>
typename AbstractManager<ContainerT, Ty>::const_iterator AbstractManager<ContainerT, Ty>::cbegin() const
{
	return m_container.cbegin();
}

template<template<class T> class ContainerT, class Ty>
typename AbstractManager<ContainerT, Ty>::const_iterator AbstractManager<ContainerT, Ty>::cend() const
{
	return m_container.cend();
}

template<template<class T> class ContainerT, class Ty>
typename AbstractManager<ContainerT, Ty>::iterator AbstractManager<ContainerT, Ty>::begin()
{
	return m_container.begin();
}

template<template<class T> class ContainerT, class Ty>
typename AbstractManager<ContainerT, Ty>::iterator AbstractManager<ContainerT, Ty>::end()
{
	return m_container.end();
}

template<template<class T> class ContainerT, class Ty>
constexpr void AbstractManager<ContainerT, Ty>::push_back(const value_type &value)
{
	if constexpr (!std::is_abstract_v<value_type>)
	{
		std::unique_ptr<value_type> p = nullptr;
		p.reset(new value_type(value));
		m_container.push_back(std::move(p));
		onPushValue(--m_container.end());
	}
}

template<template<class T> class ContainerT, class Ty>
constexpr void AbstractManager<ContainerT, Ty>::push_back(value_type &&value) noexcept
{
	if constexpr (!std::is_abstract_v<value_type>)
	{
		std::unique_ptr<value_type> p = nullptr;
		p.reset(new value_type(std::move(value)));
		m_container.push_back(std::move(p));
		onPushValue(--m_container.end());
	}
}

template<template<class T> class ContainerT, class Ty>
template<class T, class... Args>
constexpr void
AbstractManager<ContainerT, Ty>::emplace_back(const after_emplace_func<T *> &after_emplace, Args &&...args)
{
	m_container.emplace_back(new T(std::move(args)...));
	if (auto t = dynamic_cast<T *>(m_container.back().get()))
		after_emplace(t);
	onPushValue(--m_container.end());
}

template<template<class T> class ContainerT, class Ty>
template<class T, class... Args>
constexpr void
AbstractManager<ContainerT, Ty>::emplace_back(const after_emplace_func<T *> &after_emplace, const Args &...args)
{
	m_container.emplace_back(new T(args...));
	if (auto t = dynamic_cast<T *>(m_container.back().get()))
		after_emplace(t);
	onPushValue(--m_container.end());
}

template<template<class T> class ContainerT, class Ty>
typename AbstractManager<ContainerT, Ty>::iterator AbstractManager<ContainerT, Ty>::erase(const_iterator index)
{
	return m_container.erase(index);
}

template<template<class T> class ContainerT, class Ty>
size_t AbstractManager<ContainerT, Ty>::size() const
{
	return m_container.size();
}

template<template<class T> class ContainerT, class Ty>
void AbstractManager<ContainerT, Ty>::clear()
{
	m_container.clear();
}

template<template<class T> class ContainerT, class Ty>
void AbstractManager<ContainerT, Ty>::update()
{
	for (auto item = begin(); item != end(); ++item)
	{
		if (*item)
		{
			if (need_erase(item))
				item = --erase(item);
			else
				update_item(item);
		}
	}
}
