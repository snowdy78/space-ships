#pragma once

#include "AbstractLevel.hpp"
#include "AbstractLevel__StarsSprite.hpp"

class AbstractLevel::Header final : public rn::MonoBehaviour
{
	using ms = std::chrono::milliseconds;
	constexpr static ms incoming_time{ 1000 };
	constexpr static ms incoming_delay{ 3500 };
	constexpr static unsigned charsize = 58u;
	soft_parameter<float> m_appear{ 0.f, 1.f, incoming_time };
	soft_parameter<float> m_disapear{ 1.0f, 0.0f, incoming_time, incoming_delay };
	sf::Text m_text;
	StarsSprite m_stars;
	bool m_shown = true;

public:
	Header(const Difficulty &difficulty)
		: m_text("", *Font::Default, charsize),
		  m_stars(difficulty)
	{
	}
	void start() override
	{
		reset();
		const auto descr_size = m_text.getGlobalBounds().getSize();
		const int twidth	  = static_cast<int>(std::round(descr_size.x));
		m_stars.setSize(static_cast<int>(static_cast<double>(charsize) * 1.33));
		rn::Grid table{ 2, 1, getSize() };
		table.setColumnWidth(0, static_cast<float>(twidth));
		table.setColumnWidth(1, m_stars.getSize().x);
		auto descr_cell = table.getCellGlobal(0, 0);
		auto star_cell	= table.getCellGlobal(1, 0);
		m_text.setPosition(
			rn::math::centerPadding(sf::FloatRect{ descr_cell.getPosition(), descr_cell.getSize() }, descr_size)
		);

		m_stars.setPosition(
			rn::math::centerPadding(sf::FloatRect{ star_cell.getPosition(), star_cell.getSize() }, m_stars.getSize())
		);
	}
	void update() override
	{
		if (!m_shown)
		{
			if (!(m_appear.strict() && m_disapear.strict()))
			{
				auto &parameter = m_disapear.strict() ? m_appear : m_disapear;
				sf::Uint8 alpha = static_cast<sf::Uint8>(255 * parameter);
				sf::Color clr{ 255, 255, 255, alpha };
				m_stars.setFillColor(clr);
				m_text.setFillColor(clr);
			}
			if (m_appear.strict() && m_disapear.is_stopped())
			{
				m_disapear.start();
			}
			else if (m_appear.strict() && m_disapear.strict())
			{
				m_shown = true;
			}
		}
	}
	void reset()
	{
		m_shown   = false;
		m_appear   = soft_parameter(0.f, 1.f, incoming_time);
		m_disapear = soft_parameter(1.f, 0.f, incoming_time, incoming_delay);
		m_appear.start();
	}
	bool shown() const
	{
		return m_shown;
	}
	void setString(const sf::String &string)
	{
		m_text.setString(string);
	}
	const sf::String &getString() const
	{
		return m_text.getString();
	}
	rn::Vec2f getSize() const
	{
		auto ss	  = m_stars.getSize();
		auto tlbs = m_text.getLocalBounds().getSize();
		return { ss.x + tlbs.x, std::max(tlbs.y, ss.y) };
	}
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		states.transform *= getTransform();
		target.draw(m_text, states);
		target.draw(m_stars, states);
	}
};
