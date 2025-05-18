#pragma once

#include "components/effects/GlowingEffect.hpp"
#include "decl.hpp"
#include "game/Bullet.hpp"
#include "game/GameObjectBase.hpp"

struct bullet_sound : SoundDisperseEntity
{
    using SoundDisperseEntity::SoundDisperseEntity;
    void start() override;
};

class BaseBullet : public Bullet, public GameObjectBase<BaseBullet>
{
	static constexpr float initial_damage = 25.f;
	inline static loading<sf::Texture> texture
		= FileLoader::Instance().addTextureToUpload("img/bullet_shoot.png").get();
	inline static loading<sf::SoundBuffer> fly_buf = FileLoader::Instance().addSoundToUpload("./assets/bullet_fly.wav").get();
	bullet_sound fly_sound{300, 150, fly_buf};
public:
	using Bullet::Bullet;
    const sf::Texture &initTexture() const override;
	void start() override;
	void update() override;
	void onMove() override;
	void summonCopy(SpaceField &field) const override;
};
