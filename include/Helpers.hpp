#pragma once

#include "decl.hpp"

sf::SoundBuffer loadSound(const sf::String &file_name);
/**
 * @brief if clock is running then it will check on every frame does it over rollback 
 * @param clock Stopwatch to check
 * @param t modulo value
 * @return True if delay is over and stop clock
 */
bool everyTime(rn::Stopwatch &clock, const float t);

std::ostream &operator<<(std::ostream &os, const sf::Vector2f &v);
std::ostream &operator<<(std::ostream &os, const sf::FloatRect &v);
std::ostream &operator<<(std::ostream &os, const sf::View &v);

/**
 * @brief dynamic_unique_cast From stackoverflow
 * @tparam To 
 * @tparam From 
 * @tparam Deleter 
 * @param p 
 * @return 
 */
template<typename To, typename From>
std::unique_ptr<To> dynamic_unique_cast(std::unique_ptr<From> &&p)
{
	if (To *cast = dynamic_cast<To *>(p.get()))
	{
		std::unique_ptr<To> result(cast);
		p.release();
		return result;
	}
	return std::unique_ptr<To>(nullptr); // or throw std::bad_cast() if you prefer
}
