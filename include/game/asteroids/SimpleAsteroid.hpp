#pragma once

#include "components/FileLoader.hpp"
#include "game/AbstractAsteroid.hpp"
#include "game/GameObjectBase.hpp"
#include "components/GameConfiguration.hpp"

class SimpleAsteroid final : public AbstractAsteroid, public GameObjectBase<SimpleAsteroid>
{
public:
	SimpleAsteroid();
	void start() override;
	void summonCopy(SpaceField &field) const override;
	void updateCollider() override;

private:
	inline static auto &self_config = config::instance().get("SimpleAsteroid");
public:
	G_CONFIG_PROP_DEFINE(self_config, velocity);
	inline static loading<sf::Texture> texture = FileLoader::Instance().addTextureToUpload("img/asteroid.png").get();
};
