#pragma once

#include "components/FileLoader.hpp"
#include "components/GameConfiguration.hpp"
#include "game/AbstractAsteroid.hpp"
#include "game/GameManager.hpp"
#include "game/GameObjectBase.hpp"
#include "game/SpaceField.hpp"

template<class Derived, char const *ClassName, char const *PathToSprite, char const *HitSoundPath>
class AsteroidBase : public AbstractAsteroid, public GameObjectBase<Derived>
{
public:
	AsteroidBase();
	void start() override;
	void summonCopy(SpaceField &field) const override;
	void onCollisionEnter(const Collidable *collidable) override;
	void updateCollider() override;

private:
	inline static loading<sf::Texture> texture				= TextureLoader::instance().addToUpload(PathToSprite).get();
	inline static loading<sf::SoundBuffer> hit_sound_buffer = SoundLoader::instance().addToUpload(HitSoundPath).get();
	struct props
	{
		inline static auto &self_config = config::instance().get(ClassName);
		G_CONFIG_PROP_DEFINE(self_config, velocity);
		G_CONFIG_PROP_DEFINE(self_config, mass);
		G_CONFIG_PROP_DEFINE(self_config, acceleration);
		G_CONFIG_PROP_DEFINE(self_config, damage);
		G_CONFIG_PROP_DEFINE(self_config, hit_sound_lerped_pure_value);
		G_CONFIG_PROP_DEFINE(self_config, hit_sound_disperse_radius);
	};
	SoundDisperseTraits m_hit_sound_traits{ *props::hit_sound_lerped_pure_value, *props::hit_sound_disperse_radius };
};

template<class Derived, char const *ClassName, char const *PathToSprite, const char *HitSoundPath>
AsteroidBase<Derived, ClassName, PathToSprite, HitSoundPath>::AsteroidBase()
	: AbstractAsteroid(*texture)
{
}

template<class Derived, char const *ClassName, char const *PathToSprite, const char *HitSoundPath>
void AsteroidBase<Derived, ClassName, PathToSprite, HitSoundPath>::start()
{
	AbstractAsteroid::start();
	setVelocity(*props::velocity);
	setMass(*props::mass);
	setAcceleration(*props::acceleration);
	setDamage(*props::damage);
}

template<class Derived, char const *ClassName, char const *PathToSprite, const char *HitSoundPath>
void AsteroidBase<Derived, ClassName, PathToSprite, HitSoundPath>::summonCopy(SpaceField &field) const
{
	field.summonAsteroid<Derived>([this](Derived &asteroid) {
		asteroid.setPosition(getPosition());
		asteroid.setDirection(getDirection());
		asteroid.setVelocity(getVelocity());
		asteroid.setMass(getMass());
		asteroid.setAcceleration(getAcceleration());
	});
}

template<class Derived, char const *ClassName, char const *PathToSprite, char const *HitSoundPath>
void AsteroidBase<Derived, ClassName, PathToSprite, HitSoundPath>::onCollisionEnter(const Collidable *collidable)
{
	AbstractAsteroid::onCollisionEnter(collidable);
	if (GameManager::exist())
	{
		GameManager::instance().sound_manager.emplace_back<SoundDisperseEntity>(
			[this](SoundDisperseEntity &sound) {
				sound.setPosition({ getPosition().x, getPosition().y, 0 });
			},
			m_hit_sound_traits, hit_sound_buffer
		);
	}
}

template<class Derived, char const *ClassName, char const *PathToSprite, const char *HitSoundPath>
void AsteroidBase<Derived, ClassName, PathToSprite, HitSoundPath>::updateCollider()
{
	rn::math::rectangle rect{ getGlobalBounds() };
	collider.transform(rect);
}
