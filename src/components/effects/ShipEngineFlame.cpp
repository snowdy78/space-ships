#include "components/effects/ShipEngineFlame.hpp"
#include "game/ships/AbstractShip.hpp"

ShipEngineFlame::ShipEngineFlame(): m_effect(*static_effect)
{
	m_effect.setDuration(std::chrono::seconds(1));
	m_effect.setRepeating(true);
}

rn::Vec2f ShipEngineFlame::getSize() const
{
	if (m_effect.getTexture())
		return rn::Vec2f{m_effect.getTexture()->getSize()};
	return {};
}

void ShipEngineFlame::start()
{
	m_effect.start();
}

void ShipEngineFlame::update()
{
	m_effect.update();
}

void ShipEngineFlame::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_effect, states);
}
