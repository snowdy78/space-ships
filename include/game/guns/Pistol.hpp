#pragma once

#include "SoundDisperseEntity.hpp"
#include "components/FileLoader.hpp"
#include "game/GameObject.hpp"
#include "game/AbstractWeapon.hpp"
#include "game/GameObjectBase.hpp"
#include "components/GameConfiguration.hpp"

class Pistol final : public AbstractWeapon, public GameObjectBase<Pistol>
{
	inline static loading<sf::SoundBuffer> sound_buffer = SoundLoader::instance().addToUpload("shoot.ogg").get();
	struct props
	{
		inline static auto &self_config = config::instance().get("Pistol");
		G_CONFIG_PROP_DEFINE(self_config, bullet_random_spray_angle);
		G_CONFIG_PROP_DEFINE(self_config, shoot_clear_sound_distance);
		G_CONFIG_PROP_DEFINE(self_config, shoot_disperse_radius);
		G_CONFIG_PROP_DEFINE(self_config, rollback);
	};

	float disperse_angle = *props::bullet_random_spray_angle;
	SoundDisperseTraits shoot_sound_traits{ *props::shoot_clear_sound_distance, *props::shoot_disperse_radius };

public:
	using AbstractWeapon::AbstractWeapon;
	size_t bullet() const override;
	rn::Vec2f getTrajectory() const override;
	AbstractWeapon *copy() const override;
	void onShoot() override;

	std::chrono::milliseconds getRollback() const override;
};
