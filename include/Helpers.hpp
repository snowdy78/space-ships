#pragma once

#include "decl.hpp"
#include "game/GameManager.hpp"
#include "game/AbstractAsteroid.hpp"
#include <thread>
#include <list>
#include "game/actions/AbstractSummonAction.hpp"
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

template<const size_t ThreadCount, class Iter, class Func>
void multithread_for_each(Iter start, const Iter &stop, Func body)
{
	size_t distance		  = std::distance(start, stop);
	size_t size_on_thread = distance / ThreadCount;
	std::list<std::thread> threads;
	size_t remainder_iterations = distance - size_on_thread * ThreadCount;
	std::list<std::pair<Iter, Iter>> array_thread_limits;
	auto thread_start = start;
	for (size_t i = 0; i < ThreadCount; ++i)
	{
		auto thread_end	  = thread_start;
		size_t additional = 0;
		if (remainder_iterations > 0)
		{
			--remainder_iterations;
			additional = 1;
		}
		std::advance(thread_end, size_on_thread + additional);
		array_thread_limits.emplace_back(thread_start, thread_end);
		thread_start = thread_end;
	}
	for (auto &thread_limit: array_thread_limits)
	{
		threads.emplace_back(
			[&body](const Iter &start, const Iter &stop) {
				std::for_each(start, stop, body);
			},
			thread_limit.first, thread_limit.second
		);
	}
	for (auto &thread: threads)
		thread.join();
}

rn::Vec2f randomAreaPoint(const sf::FloatRect &area);

rn::Vec2f randomPointOutsideArea(const sf::FloatRect &area, float distance_from_area = 100.f);

void randomBodyDirectionalOnAreaOutsideArea(
	const sf::FloatRect &view_area, RigitBody2d &body, rn::Vec2f body_size = {}
);
template<AsteroidConcept ...Args>
void randomlySummonAsteroidOutsideArea(const sf::FloatRect &view_area)
{
	if (!GameManager::exist())
	{
		throw std::runtime_error("game session does not exist! (randomlySummonAsteroidOutsideArea)\n");
	}
	std::vector<size_t> asteroid_ids			 = { Args::identifier... };
	GameManager::session()->action_manager.emplaceToTop<SummonAsteroidAction>(
		*rn::random::item(asteroid_ids.begin(), asteroid_ids.end()), [view_area](AbstractAsteroid &asteroid) {
			auto asize				   = asteroid.getSize();
			randomBodyDirectionalOnAreaOutsideArea(view_area, asteroid, asize);
		}
	);

}
