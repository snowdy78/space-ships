#pragma once

#include "game/AbstractAsteroid.hpp"
#include "game/GameObjectBase.hpp"
#include "components/GameConfiguration.hpp"
#include "components/FileLoader.hpp"
#include "game/SpaceField.hpp"

template<class Derived, char const *ClassName, char const *PathToSprite>
class AsteroidBase : public AbstractAsteroid, public GameObjectBase<Derived>
{
public:
	AsteroidBase();
	void start() override;
	void summonCopy(SpaceField &field) const override;
	void updateCollider() override;

private:
	inline static loading<sf::Texture> texture = TextureLoader::instance().addToUpload(PathToSprite).get();
	struct props
	{
		inline static auto &self_config = config::instance().get(ClassName);
		G_CONFIG_PROP_DEFINE(self_config, velocity);
		G_CONFIG_PROP_DEFINE(self_config, mass);
		G_CONFIG_PROP_DEFINE(self_config, acceleration);
	};
};

template<class Derived, char const *ClassName, char const *PathToSprite>
AsteroidBase<Derived, ClassName, PathToSprite>::AsteroidBase()
	: AbstractAsteroid(*texture)
{

}

template<class Derived, char const *ClassName, char const *PathToSprite>
void AsteroidBase<Derived, ClassName, PathToSprite>::start()
{
	AbstractAsteroid::start();
	setVelocity(*props::velocity);
	setMass(*props::mass);
	setAcceleration(*props::acceleration);
}

template<class Derived, char const *ClassName, char const *PathToSprite>
void AsteroidBase<Derived, ClassName, PathToSprite>::summonCopy(SpaceField &field) const
{
	field.summonAsteroid<Derived>([this](Derived &asteroid) {
		asteroid.setPosition(getPosition());
		asteroid.setDirection(getDirection());
		asteroid.setVelocity(getVelocity());
		asteroid.setMass(getMass());
		asteroid.setAcceleration(getAcceleration());
	});
}

template<class Derived, char const *ClassName, char const *PathToSprite>
void AsteroidBase<Derived, ClassName, PathToSprite>::updateCollider()
{
	rn::math::rectangle rect{ getGlobalBounds() };
	collider.transform(rect);
}
