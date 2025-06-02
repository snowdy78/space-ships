#pragma once

#include "decl.hpp"
#include "game/GameManager.hpp"

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

rn::Vec2f randomAreaPoint(const sf::FloatRect &area);

template<class T>
void randomlySummonAsteroidOutsideArea(const sf::FloatRect &view_area, float velocity)
{
	using rn::math::coordinates2d;
	using rn::math::degrees, rn::math::radians;
	using rn::math::sgn;

	AbstractAsteroid *asteroid = GameManager::session()->field.summonAsteroid<T>(rn::Vec2f{}, rn::Vec2f{});
	auto asize				   = asteroid->getSize();
	int random_side			   = rn::random::integer(0, 3);
	float k					   = rn::random::real<float>(0, 1);
	rn::math::rectangle view_rect(view_area);
	rn::math::rectangle rect(view_area.getPosition() - asize, view_area.getSize() + asize * 2.f);
	// random position by random point on random side
	rn::Vec2f position = rect.side(random_side).lerp(k);

	rn::Vec2f direction_point1 = view_rect.point(random_side == 0 ? rect.point_count() - 1 : random_side - 1);
	rn::Vec2f direction_point2 = view_rect.point(random_side == 3 ? 0 : random_side + 1);
	rn::Vec2f view_area_center = view_area.getPosition() + view_area.getSize() / 2.f;
	// finding random direction by two neighbour points of random side first point
	degrees random_angle  = rn::random::real<float>(0, 1);
	degrees base_angle	  = rn::math::rot(direction_point2 - position);
	degrees max_add_angle = rn::math::angle_of(direction_point1, position, direction_point2);
	base_angle -= max_add_angle * random_angle;
	rn::Vec2f dir = direction(base_angle);

	asteroid->setDirection(rn::math::norm(dir));
	asteroid->setVelocity(velocity);
	asteroid->setPosition(position);
#ifdef SPACE_SHIP_DEBUG
	std::cout << "summon asteroid in randomly place: {";
	std::cout << "position " << to_json(position) << ", ";
	std::cout << "direction " << to_json(dir) << ", ";
	std::cout << "angle " << base_angle << ", ";
	std::cout << "}\n";
#endif
}
