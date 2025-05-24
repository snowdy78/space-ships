#pragma once

#include "game/GameManager.hpp"
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


template<class T>
void randomlySummonAsteroidOutsideArea(const sf::FloatRect &view_area, float velocity)
{
	using rn::math::coordinates2d;
	using rn::math::degrees;
	using rn::math::sgn;
	constexpr float indent_length  = 100;
	constexpr float angle_disperse = 30;
	int random_side				   = rn::random::integer(0, 3);
	float teta					   = rn::random::real(0.f, 1.f);
	float random_angle			   = rn::random::real(-0.5f, 0.5f);
	rn::math::rectangle rect(view_area);
	auto side = rect.side(random_side);
	auto rc	  = rect.center();
	auto n	  = nearest(rc, side.p1, side.p2);
	auto d	  = norm(n);
	rn::Vec2f pos
		= rc + n
		  + d * indent_length
				* (rn::math::approx<4>()((rc - n).y) == 0 ? rn::math::vec2{ teta, 1.f } : rn::math::vec2(1.f, teta));
	auto angle = rot(norm(rc - pos));
	angle += degrees(angle_disperse * random_angle);
	rn::Vec2f vel = velocity * direction(angle);
	if (GameManager::exist())
		GameManager::session()->field.summonAsteroid<T>(pos, vel);
}

