#include "game/Camera2d.hpp"

sf::View Camera2d::getView() const
{
	return sf::View(getViewRect());
}

sf::FloatRect Camera2d::getViewRect() const
{
	return getTransform().transformRect({ {}, getViewSize() });
}

Camera2d::Camera2d()
	: m_viewsize(rn::VideoSettings::getResolution())
{}

void Camera2d::setPosition(const rn::Vec2f &v)
{
	rn::Vec2f p = getPosition();
	sf::Transformable::setPosition(v);
	onCameraMove(p);
}


void Camera2d::setPosition(float x, float y)
{
	rn::Vec2f p = getPosition();
	sf::Transformable::setPosition(x, y);
	onCameraMove(p);
}

void Camera2d::move(const rn::Vec2f &v)
{
	auto p = getPosition();
	sf::Transformable::move(v);
	onCameraMove(p);
}

void Camera2d::move(float x, float y)
{
	auto p = getPosition();
	sf::Transformable::move(x, y);
	onCameraMove(p);
}

rn::Vec2f Camera2d::getViewSize() const
{
	return m_viewsize;
}
