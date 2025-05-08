#pragma once

#include <chrono>
#include "components/Effect.hpp"
#include "decl.hpp"


class GlowingEffect : public Effect
{
	constexpr static const char *shader_program = R"(
        #version 120
        
        uniform float uMaxGlowing;
        uniform float uMinGlowing;
        uniform float uGlowSpeed;
        uniform vec4 uColor;
        uniform vec2 uResolution; // resolution
        uniform float uTime;	  // time

        void main()
        {
            vec2 uv = gl_FragCoord.xy/uResolution.xy*2.0 - 0.5;
            vec4 color = uColor;
            vec2 pos = vec2(0.5);
            float smax = uMaxGlowing;
            float playspeed = uGlowSpeed;
            float smin = uMinGlowing;
            gl_FragColor = vec4(length(pos - uv) / (abs(cos(uTime*playspeed)) * smin - smax) + color);
        }
    )";
	bool m_is_played							= false;
	std::unique_ptr<sf::Shader> m_shader = nullptr;
	sf::RenderTexture m_surface;
	rn::Stopwatch m_clock;
	float m_max_glow_radius = 1.0;
	sf::Color m_color{sf::Color::White};
	float m_glow_speed = 1.0;
    float m_min_glow_radius = 0.5;
    void update_uniforms()
    {
        if (m_shader)
        {
            m_shader->setUniform("uResolution", getSize());
            m_shader->setUniform("uTime", static_cast<float>(m_clock.time<std::chrono::milliseconds>().count()) / 1000.f);
            m_shader->setUniform("uColor", sf::Glsl::Vec4{m_color});
            m_shader->setUniform("uMaxGlowing", m_max_glow_radius);
            m_shader->setUniform("uMinGlowing", m_min_glow_radius);
            m_shader->setUniform("uGlowSpeed", m_glow_speed);
        }
    }
public:
    GlowingEffect() 
    {
    }
	GlowingEffect(const rn::Vec2f &size)
	{
		rn::Vec2u s{ size };
		m_surface.create(s.x, s.y);
		setTexture(m_surface.getTexture());
        m_shader.reset(new sf::Shader);
        m_shader->loadFromMemory(shader_program, sf::Shader::Fragment);
        m_surface.clear(sf::Color::Transparent);
        update_uniforms();
	}
	void played(bool is_played)
	{
		m_is_played = is_played;
	}
	void setMaxGlowRadius(float radius)
	{
		m_max_glow_radius = radius;
        if (m_shader)
            m_shader->setUniform("uMaxGlowing", m_max_glow_radius);
	}
	float getMaxGlowRadius() const
	{
		return m_max_glow_radius;
	}
    void setMinGlowRadius(float radius)
    {
        m_min_glow_radius = radius;
        if (m_shader)
            m_shader->setUniform("uMinGlowing", m_min_glow_radius);
    }
    float getMinGlowRadius() const
    {
        return m_min_glow_radius;
    }
    void setGlowSpeed(float play_speed)
    {
        m_glow_speed = play_speed;
        if (m_shader)
            m_shader->setUniform("uGlowSpeed", m_glow_speed);
    }
    float getGlowSpeed() const
    {
        return m_glow_speed;
    }
    void setFillColor(const sf::Color &color)
    {
        m_color = color;
        if (m_shader)
            m_shader->setUniform("uColor", sf::Glsl::Vec4{m_color});
    }
    void start() override
    {
        m_clock.start();
    }
	void update() override
	{
        if (m_shader)
        {
            if (m_max_glow_radius != m_min_glow_radius)
            {
                m_shader->setUniform("uTime", static_cast<float>(m_clock.time<std::chrono::milliseconds>().count()) / 1000.f);
            }
            m_surface.clear(sf::Color::Transparent);
            m_surface.draw(*this, &*m_shader);
        }
	}
    rn::Vec2f getSize() const
    {
        return rn::Vec2f{m_surface.getSize()};
    }
    void setSize(const rn::Vec2f &size)
    {
        m_surface.create(static_cast<unsigned>(size.x), static_cast<unsigned>(size.y));
		setTexture(m_surface.getTexture());
        m_surface.clear(sf::Color::Transparent);
		if (!m_shader)
        {
            m_shader.reset(new sf::Shader);
            m_shader->loadFromMemory(shader_program, sf::Shader::Fragment);
            update_uniforms();
        }
    }
	bool played() const override
	{
		return m_is_played;
	}
};
