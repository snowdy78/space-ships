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
	inline static auto &self_init			 = config::instance().get("BaseBullet");
	inline static auto &initial_damage		 = self_init.get("damage");
	inline static auto &clear_sound_distance = self_init.get("clear_sound_distance");
	inline static auto &disperse_force		 = self_init.get("disperse_force");
	inline static loading<sf::Texture> texture
		= TextureLoader::instance().addToUpload("img/bullet_shoot.png").get();
	inline static loading<sf::SoundBuffer> fly_buf
		= SoundLoader::instance().addToUpload("./assets/bullet_fly.wav").get();
	bullet_sound fly_sound{ *clear_sound_distance, *disperse_force, fly_buf };

public:
	using AbstractBullet::AbstractBullet;
	const sf::Texture &initTexture() const override;
	void start() override;
	void update() override;
	void onMove() override;
	void summonCopy(SpaceField &field) const override;
};
