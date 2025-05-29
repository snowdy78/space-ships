#include "Helpers.hpp"

sf::SoundBuffer loadSound(const sf::String &file_name)
{
	sf::SoundBuffer sound_buffer;
	if (!sound_buffer.loadFromFile(file_name))
	{
		std::cerr << "Failed to load sound file 'shoot.ogg'" << std::endl;
		throw std::exception();
	}
	return std::move(sound_buffer);
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

bool everyTime(rn::Stopwatch &clock, const float t)
{
	if (clock.is_stopped())
		return false;
	if (clock.time<std::chrono::milliseconds>().count() > t)
	{
		clock.stop();
		return true;
	}
	return false;
}
