#pragma once

#include "GameConfiguration.hpp"
#include "decl.hpp"

class Background : public rn::MonoBehaviour
{
	static loading<sf::Texture> background_image1;
	struct props
	{
		inline static auto &self_config = config::instance().get("Background");
		G_CONFIG_PROP_DEFINE(self_config, velocity_layer1);
		G_CONFIG_PROP_DEFINE(self_config, velocity_layer2);
	};
	rn::Vec2u res = rn::VideoSettings::getResolution();
	rn::ShaderTexture shader{ res, "space.frag", sf::Shader::Fragment };
	rn::Vec2f m_shader_position;
	rn::Vec2f m_layer1_position;
	sf::Sprite layer1;
	sf::Clock clock;

	static rn::Vec2f notZeroPosition(const rn::Vec2f &p, const rn::Vec2f &els);

public:
	Background() = default;
	template<class T>
	void setUniform(const sf::String &name, const T &value);
	void setPosition(const rn::Vec2f &pos);
	void setPosition(float x, float y);
	void move(const rn::Vec2f &);
	void move(float, float);
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void onMove(const rn::Vec2f &pos_before);
};
template<class T>
void Background::setUniform(const sf::String &name, const T &value)
{
	shader.setUniform(name, value);
}
