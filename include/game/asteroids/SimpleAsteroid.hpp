#pragma once

#include "components/FileLoader.hpp"
#include "game/AbstractAsteroid.hpp"
#include "game/SpaceField.hpp"

class SimpleAsteroid final : public AbstractAsteroid
{
public:
	SimpleAsteroid();
	void summonCopy(SpaceField &field) const override;
	void updateCollider() override;

private:
	inline static loading<sf::Texture> texture = FileLoader::Instance().addTextureToUpload("img/asteroid.png").get();
};
