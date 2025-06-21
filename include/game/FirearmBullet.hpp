#pragma once

#include "components/FileLoader.hpp"
#include "components/GameConfiguration.hpp"
#include "decl.hpp"
#include "game/AbstractBullet.hpp"
#include "game/GameObjectBase.hpp"

template<class Derived, class HitEffect, char const *SpritePath, char const *ClassName>
class FirearmBullet : public AbstractBullet, public GameObjectBase<Derived>
{
	struct props
	{
		inline static auto &self_config = config::instance().get(ClassName);
		G_CONFIG_PROP_DEFINE(self_config, damage);
		G_CONFIG_PROP_DEFINE(self_config, fly_clear_sound_distance);
		G_CONFIG_PROP_DEFINE(self_config, fly_disperse_radius);
		G_CONFIG_PROP_DEFINE(self_config, velocity);
	};
	inline static loading<sf::Texture> texture = TextureLoader::instance().addToUpload(SpritePath).get();

public:
	using AbstractBullet::AbstractBullet;
	const sf::Texture &initTexture() const override;
	void start() override;
	void onDestroy() override;
	void summonCopy(SpaceField &field) const override;
};

template<class T, class HitEffect, char const* SpritePath, char const* ClassName>
const sf::Texture & FirearmBullet<T, HitEffect, SpritePath, ClassName>::initTexture() const
{
	return *texture;
}

template<class T, class HitEffect, char const *SpritePath, char const *ClassName>
void FirearmBullet<T, HitEffect, SpritePath, ClassName>::start()
{
	this->setDamage(*props::damage);

	float mass		   = 0.100f;
	float velocity	   = *props::velocity;
	float acceleration = 1.f / 2500.f;

	setVelocity(velocity);
	setMass(mass);
	setAcceleration(acceleration);
	setRotation(rn::math::rot(getDirection()));

	AbstractBullet::start();
}

template<class Derived, class HitEffect, char const* SpritePath, char const* ClassName>
void FirearmBullet<Derived, HitEffect, SpritePath, ClassName>::onDestroy()
{
	AbstractBullet::onDestroy();
	if (GameManager::exist())
	{
		GameManager::instance().effect_manager.emplace_back<HitEffect>([this](HitEffect &effect) {
			effect.setPosition(getPosition());
		});
	}
}

template<class T, class HitEffect, char const *SpritePath, char const *ClassName>
void FirearmBullet<T, HitEffect, SpritePath, ClassName>::summonCopy(SpaceField &field) const
{
	field.summonBullet<T>([this](T &bullet) {
		bullet.author_ptr = author_ptr;
		bullet.setDirection(getDirection());
	});
}
