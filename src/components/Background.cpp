#include "components/Background.hpp"
#include "components/FileLoader.hpp"

loading<sf::Texture> Background::background_image1 = TextureLoader::instance().addToUpload("img/background1.png").get();

rn::Vec2f Background::notZeroPosition(const rn::Vec2f &p, const rn::Vec2f& els)
{
	return p == rn::Vec2f{} ? rn::Vec2f{} : els;
}

void Background::setPosition(const rn::Vec2f &pos)
{
	const auto p = getPosition();
	Transformable::setPosition(pos);
	onMove(p);
}

void Background::setPosition(float x, float y)
{
	const auto p = getPosition();
	Transformable::setPosition(x, y);
	onMove(p);
}

void Background::move(const rn::Vec2f &offset)
{
	const auto p = getPosition();
	Transformable::move(offset);
	onMove(p);
}

void Background::move(float offset_x, float offset_y)
{
	const auto p = getPosition();
	Transformable::move(offset_x, offset_y);
	onMove(p);
}

void Background::start()
{
	setPosition(0, 0);
	std::cout << *props::velocity_layer1 << "\n";
	std::cout << *props::velocity_layer2 << "\n";
	layer1.setTexture(*background_image1);
	shader.setUniform("iResolution", rn::Vec2f(res));
}

void Background::update()
{
	shader.setUniform("iTime", static_cast<float>(clock.getElapsedTime().asMilliseconds()) / 1000.f);
}


void Background::onEvent(sf::Event &event)
{
}

void Background::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(shader, states);
	target.draw(layer1, states);
}

void Background::onMove(const rn::Vec2f &pos_before)
{
	float v1 = *props::velocity_layer1, v2 = *props::velocity_layer2;
	m_shader_position -= notZeroPosition(getPosition(), rn::math::norm(getPosition() - pos_before) * v1);
	shader.setUniform<rn::Vec2f>("iPosition", m_shader_position);
	shader.setPosition(getPosition());
	m_layer1_position -= notZeroPosition(getPosition(), rn::math::norm(getPosition() - pos_before) * v2);
	layer1.setPosition(m_layer1_position);
}
