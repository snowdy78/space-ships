#include "components/Background.hpp"

void Background::start()
{
	shader.setUniform("iResolution", rn::Vec2f(res));
}

void Background::update()
{
	shader.setUniform("iPosition", -getPosition());
	shader.setUniform("iTime", clock.getElapsedTime().asMilliseconds() / 1000.f);
}

void Background::onEvent(sf::Event &event) {}

void Background::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(shader, states);
}
