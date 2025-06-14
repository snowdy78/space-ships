#pragma once

#include "components/AnimatedSprite.hpp"
#include "components/FileLoader.hpp"
#include "game/RigitBody2d.hpp"

class ShipEngineFlame : public rn::MonoBehaviour
{
	inline static loading<AnimatedSprite> static_effect
		= AnimationLoader::instance().addToUpload("./img/animation/ship-engine/.png").get();

	friend class AbstractShip;

public:
	ShipEngineFlame();

	rn::Vec2f getSize() const;
	void start() override;
	void update() override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	AnimatedSprite m_effect;
};
