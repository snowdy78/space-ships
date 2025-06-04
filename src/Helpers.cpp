#include "Helpers.hpp"
#include <optional>
rn::Json to_json(const sf::Transformable &transformable)
{
	return {
		{ "position", to_json(transformable.getPosition()) },
		{ "rotation", transformable.getRotation()		  },
		{ "scale",	   to_json(transformable.getScale())	 },
		{ "origin",	to_json(transformable.getOrigin())   }
	};
}

rn::Json to_json(const RigitBody2d &body)
{
	const sf::Transformable &t = static_cast<const sf::Transformable &>(body);
	auto i					   = rn::Json{
							{ "velocity",	  body.getVelocity()			 },
							{ "direction",	   to_json(body.getDirection()) },
							{ "acceleration", body.getAcceleration()		 },
							{ "mass",		  body.getMass()				 },
	};
	i.push_back(to_json(t));
	return i;
}


void json_assign(sf::Transformable &t, const rn::Json &json)
{
	if (!(json.contains("position") && json.contains("rotation") && json.contains("scale") && json.contains("origin")))
	{
		using namespace std::string_literals;
		throw std::out_of_range(
			"json does not contains keys ["s + (!json.contains("position") ? "position " : "")
			+ (!json.contains("rotation") ? "rotation " : "") + (!json.contains("scale") ? "scale " : "")
			+ (!json.contains("origin") ? "origin " : "") + "]"
		);
	}
	auto p = vector_from_json<float>(json["position"]);
	auto o = vector_from_json<float>(json["origin"]);
	auto s = vector_from_json<float>(json["scale"]);
	t.setPosition(p);
	t.setRotation(json["rotation"]);
	t.setOrigin(o);
	t.setScale(s);
}

void json_assign(RigitBody2d &body, const rn::Json &json)
{
	if (!(json.contains("velocity") && json.contains("direction") && json.contains("acceleration")
		  && json.contains("mass")))
	{
		using namespace std::string_literals;
		throw std::out_of_range(
			"json does not contains keys ["s + (!json.contains("velocity") ? "velocity " : "")
			+ (!json.contains("direction") ? "direction " : "")
			+ (!json.contains("acceleration") ? "acceleration " : "") + (!json.contains("mass") ? "mass " : "") + "]"
		);
	}
	json_assign(static_cast<sf::Transformable &>(body), json);
	const auto d = vector_from_json<float>(json["direction"]);
	body.setVelocity(json["velocity"]);
	body.setAcceleration(json["acceleration"]);
	body.setMass(json["mass"]);
	body.setDirection(d);
}

sf::SoundBuffer loadSound(const sf::String &file_name)
{
	sf::SoundBuffer sound_buffer;
	if (!sound_buffer.loadFromFile(file_name))
	{
		std::cerr << "Failed to load sound file 'shoot.ogg'" << '\n';
		throw std::exception();
	}
	return sound_buffer;
}
std::ostream &operator<<(std::ostream &os, const sf::Vector2f &v)
{
	os << "Vector2( x:" << v.x << ", y:" << v.y << ")";
	return os;
}

std::ostream &operator<<(std::ostream &os, const sf::FloatRect &v)
{
	os << "FloatRect( x:" << v.left << ", y:" << v.top << ", w:" << v.width << ", h:" << v.height << ")";
	return os;
}

std::ostream &operator<<(std::ostream &os, const sf::View &v)
{
	os << "sf::View( cx:" << v.getCenter().x << ", cy:" << v.getCenter().y << ", w:" << v.getSize().x
	   << ", h:" << v.getSize().y << ")";
	return os;
}

rn::Vec2f randomAreaPoint(const sf::FloatRect &area)
{
	return { rn::random::real(area.getPosition().x, area.getPosition().x + area.getSize().x),
			 rn::random::real(area.getPosition().y, area.getPosition().y + area.getSize().y) };
}
bool everyTime(const rn::Stopwatch &clock, const std::chrono::milliseconds &t)
{
	if (clock.is_stopped())
		return false;
	return clock.time<std::chrono::milliseconds>() > t;
}

bool timeStep(
	const rn::Stopwatch &clock, const std::chrono::milliseconds &t1, std::optional<std::chrono::milliseconds> t2
)
{
	using ms_t = std::chrono::milliseconds;
	using namespace std::chrono_literals;
	if (!t2.has_value())
		t2 = 2 * t1;
	const ms_t full_dist = *t2;
	const ms_t t		 = clock.time<ms_t>() % full_dist;
	if (t <= t1)
		return true;
	return false;
}
