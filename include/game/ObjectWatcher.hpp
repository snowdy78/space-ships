#pragma once

#include "decl.hpp"

/**
 * @brief the object watching out of view area
 */
class ObjectWatcher
{
	sf::FloatRect m_view_area;

public:
	ObjectWatcher(const sf::FloatRect &view_area = {});
	const sf::FloatRect &getViewArea() const
	{
		return m_view_area;
	}
	void setViewArea(const sf::FloatRect &view_area)
	{
		m_view_area = view_area;
	}
	bool isOutOfViewArea(const sf::FloatRect &object_bounds) const
	{
		return rn::math::rectangle(m_view_area).collide(rn::math::rectangle(object_bounds));
	}
};
