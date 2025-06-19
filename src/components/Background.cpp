#include "components/Background.hpp"

#include "Helpers.hpp"
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
	m_velocity_layer1 = { *props::velocity_layer1_t::x, *props::velocity_layer1_t::y };
	m_velocity_layer2 = { *props::velocity_layer2_t::x, *props::velocity_layer2_t::y };
	setPosition(0, 0);
	layer1.setTexture(*background_image1);
	layer1.setScale(3, 3);
	auto resf = rn::Vec2f(res);

	layer1.setOrigin(resf / 2.f);
	shader.setUniform("iResolution", resf);
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
	states.transform *= getTransform();
	target.draw(shader, states);
	target.draw(layer1, states);
}

void Background::onMove(const rn::Vec2f &pos_before)
{
	rn::Vec2f delta = getPosition() - pos_before;
	rn::Vec2f p		= rn::math::norm(delta);
	p.x *= m_velocity_layer1.x;
	p.y *= m_velocity_layer1.y;
	m_shader_position -= notZeroPosition(delta, p);
	shader.setUniform<rn::Vec2f>("iPosition", m_shader_position);
	p.x *= m_velocity_layer2.x;
	p.y *= m_velocity_layer2.y;
	m_layer1_position -= notZeroPosition(delta, p);
	layer1.setPosition(m_layer1_position);
}
