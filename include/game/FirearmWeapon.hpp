#pragma once

#include "SoundDisperseEntity.hpp"
#include "components/FileLoader.hpp"
#include "components/GameConfiguration.hpp"
#include "game/AbstractWeapon.hpp"
#include "game/GameObjectBase.hpp"
#include "game/GameManager.hpp"
#include "game/RigitBody2d.hpp"

template<class T>
concept IdentifiedConcept = requires() { T::identifier; };

template<class T, IdentifiedConcept, char const *SoundPath, char const *SpritePath, char const *ClassName>
class FirearmWeapon : public AbstractWeapon, public GameObjectBase<T>
{
	inline static loading<sf::SoundBuffer> sound_buffer = SoundLoader::instance().addToUpload(SoundPath).get();
	struct props
	{
		inline static auto &self_config = config::instance().get(ClassName);
		G_CONFIG_PROP_DEFINE(self_config, bullet_random_spray_angle);
		G_CONFIG_PROP_DEFINE(self_config, shoot_clear_sound_distance);
		G_CONFIG_PROP_DEFINE(self_config, shoot_disperse_radius);
		G_CONFIG_PROP_DEFINE(self_config, rollback);
		G_CONFIG_PROP_DEFINE(self_config, automatic);
	};
	inline static loading<sf::Texture> texture
		= TextureLoader::instance().addToUpload(SpritePath).get();
	float disperse_angle = *props::bullet_random_spray_angle;
	SoundDisperseTraits shoot_sound_traits{ *props::shoot_clear_sound_distance, *props::shoot_disperse_radius };
	sf::Sprite m_sprite{ *texture };

public:
	using AbstractWeapon::AbstractWeapon;
	size_t bullet() const override;
	rn::Vec2f getTrajectory() const override;
	AbstractWeapon *copy() const override;
	bool isAutomatic() const override;
	void onShoot() override;
	const sf::Drawable &getItemSprite() const override;
	std::chrono::milliseconds getRollback() const override;
};

template<class T, IdentifiedConcept BulletType, const char* SoundPath, const char* SpritePath, const char* ClassName>
size_t FirearmWeapon<T, BulletType, SoundPath, SpritePath, ClassName>::bullet() const
{
	return BulletType::identifier;
}

template<class T, IdentifiedConcept BulletType, const char* SoundPath, const char* SpritePath, const char* ClassName>
rn::Vec2f FirearmWeapon<T, BulletType, SoundPath, SpritePath, ClassName>::getTrajectory() const
{
	if (!hasOwner())
	{
		std::cerr << "cannot shoot gun owner not exist\n";
		return { 1, 0 };
	}
	auto owner_ptr = getOwner<RigitBody2d>();
	if (!owner_ptr)
	{
		std::cerr << "cannot shoot gun owner not a rigitbody2d\n";
		return { 1, 0 };
	}
	auto angle = rn::math::rot(owner_ptr->getDirection())
				 + rn::math::degrees(rn::random::real(-1.f, 1.f) * disperse_angle / 2.f);
	return direction(angle);
}

template<class T, IdentifiedConcept BulletType, const char* SoundPath, const char* SpritePath, const char* ClassName>
AbstractWeapon *FirearmWeapon<T, BulletType, SoundPath, SpritePath, ClassName>::copy() const
{
	return new T;
}

template<class T, IdentifiedConcept BulletType, const char* SoundPath, const char* SpritePath, const char* ClassName>
bool FirearmWeapon<T, BulletType, SoundPath, SpritePath, ClassName>::isAutomatic() const
{
	return *props::automatic;
}

template<class T, IdentifiedConcept BulletType, const char* SoundPath, const char* SpritePath, const char* ClassName>
void FirearmWeapon<T, BulletType, SoundPath, SpritePath, ClassName>::onShoot()
{
	if (GameManager::exist())
	{
		SoundDisperseEntity sound{ shoot_sound_traits, sound_buffer };
		GameManager::instance().sound_manager.emplace_back<SoundDisperseEntity>(
			[this](SoundDisperseEntity &sound) {
				sound.setPosition({ getPosition().x, getPosition().y, 0 });
			},
			shoot_sound_traits, sound_buffer
		);
	}
}

template<class T, IdentifiedConcept BulletType, const char* SoundPath, const char* SpritePath, const char* ClassName>
const sf::Drawable & FirearmWeapon<T, BulletType, SoundPath, SpritePath, ClassName>::getItemSprite() const
{
	return m_sprite;
}

template<class T, IdentifiedConcept BulletType, const char* SoundPath, const char* SpritePath, const char* ClassName>
std::chrono::milliseconds FirearmWeapon<T, BulletType, SoundPath, SpritePath, ClassName>::getRollback() const
{
	return std::chrono::milliseconds{ *props::rollback };
}
