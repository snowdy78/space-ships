#pragma once

#include "SoundDisperseEntity.hpp"
#include "components/FileLoader.hpp"
#include "components/GameConfiguration.hpp"
#include "decl.hpp"
#include "game/AbstractBullet.hpp"
#include "game/GameObjectBase.hpp"

struct bullet_sound : SoundDisperseEntity
{
	using SoundDisperseEntity::SoundDisperseEntity;
	void start() override;
};

class BaseBullet final : public AbstractBullet, public GameObjectBase<BaseBullet>
{
	struct props
	{
		inline static auto &self_config			 = config::instance().get("BaseBullet");
		G_CONFIG_PROP_DEFINE(self_config, damage);
		G_CONFIG_PROP_DEFINE(self_config, fly_clear_sound_distance);
		G_CONFIG_PROP_DEFINE(self_config, fly_disperse_radius);
	};
	inline static loading<sf::Texture> texture
		= TextureLoader::instance().addToUpload("img/bullet_shoot.png").get();
	inline static loading<sf::SoundBuffer> fly_buf
		= SoundLoader::instance().addToUpload("./assets/bullet_fly.wav").get();
	bullet_sound fly_sound{ *props::fly_clear_sound_distance, *props::fly_disperse_radius, fly_buf };

public:
	using AbstractBullet::AbstractBullet;
	const sf::Texture &initTexture() const override;
	void start() override;
	void update() override;
	void onMove() override;
	void summonCopy(SpaceField &field) const override;
};
