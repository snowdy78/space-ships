#pragma once

#include "decl.hpp"
#include "game/GameManager.hpp"
#include "game/AbstractAsteroid.hpp"

template<class T>
rn::Json to_json(const rn::Vec2<T> &vec)
{
	return {
		{ "x", vec.x },
		{ "y", vec.y }
	};
}

rn::Json to_json(const sf::Transformable &transformable);
rn::Json to_json(const RigitBody2d &body);
template<class T>
rn::Vec2<T> vector_from_json(const rn::Json &json)
{
	if (!json.contains("x") || !json.contains("y"))
	{
		using namespace std::string_literals;
		throw std::out_of_range(
			"json does not contains keys ["s + (!json.contains("x") ? "x " : "") + (!json.contains("y") ? "y " : "")
		);
	}
	return rn::Vec2<T>(json["x"], json["y"]);
}
void json_assign(sf::Transformable &t, const rn::Json &json);
void json_assign(RigitBody2d &body, const rn::Json &json);

sf::SoundBuffer loadSound(const sf::String &file_name);
/**
 * @brief true if time is over the `t` parameter
 * @param clock Stopwatch to check
 * @param t modulo value
 */
bool everyTime(const rn::Stopwatch &clock, const std::chrono::milliseconds &t);

/**
 * @brief true on time interval [2*n*t, t1 + 2*n*`t`], false on interval (t1 + 2*n*t, 2*t2 + 2*n*t] where n is an integer
 * step
 * @param clock
 * @param t1 - first interval time value
 * @param t2 - second interval time value
 * @return
 */
bool timeStep(
	const rn::Stopwatch &clock, const std::chrono::milliseconds &t1,
	std::optional<std::chrono::milliseconds> t2 = std::nullopt
);

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

rn::Vec2f randomPointOutsideArea(const sf::FloatRect &area, float distance_from_area = 100.f);

void randomBodyDirectionalOnAreaOutsideArea(
	const sf::FloatRect &view_area, RigitBody2d &body, float velocity, rn::Vec2f body_size = {}
);
template<class T>
void randomlySummonAsteroidOutsideArea(const sf::FloatRect &view_area, float velocity)
{
	using rn::math::coordinates2d;
	using rn::math::degrees, rn::math::radians;
	using rn::math::sgn;

	SpaceField::State<AbstractAsteroid> asteroid = GameManager::session()->field.summonAsteroid<T>().lock();
	auto asize				   = asteroid->getSize();
	randomBodyDirectionalOnAreaOutsideArea(view_area, *asteroid, velocity, asize);

}


