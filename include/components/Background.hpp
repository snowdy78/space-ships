#pragma once

#include "decl.hpp"

class Background : public rn::MonoBehaviour
{
	rn::Vec2u res = rn::VideoSettings::getResolution();
	rn::ShaderTexture shader{ res, "space.frag", sf::Shader::Fragment };
	sf::Clock clock;

public:
	Background() {}
	template<class T>
	void setUniform(const sf::String &name, const T &value);

	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
template<class T>
void Background::setUniform(const sf::String &name, const T &value)
{
	shader.setUniform(name, value);
}
