#pragma once

#include "game/AbstractAsteroid.hpp"
#include "game/GameObjectBase.hpp"
#include "components/GameConfiguration.hpp"
#include "components/FileLoader.hpp"

class SimpleAsteroid final : public AbstractAsteroid, public GameObjectBase<SimpleAsteroid>
{
public:
	SimpleAsteroid();
	void start() override;
	void summonCopy(SpaceField &field) const override;
	void updateCollider() override;

private:
	inline static loading<sf::Texture> texture = TextureLoader::instance().addToUpload("img/asteroid.png").get();
	inline static auto &self_config = config::instance().get("SimpleAsteroid");
public:
	G_CONFIG_PROP_DEFINE(self_config, velocity);
};
