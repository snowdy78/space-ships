#pragma once

#include "SoundDisperseEntity.hpp"
#include "components/FileLoader.hpp"
#include "game/GameObject.hpp"
#include "game/Gun.hpp"
#include "game/GameObjectBase.hpp"

class Pistol : public Gun, public GameObjectBase<Pistol>
{
	inline static loading<sf::SoundBuffer> sound_buffer = FileLoader::Instance().addSoundToUpload("shoot.ogg").get();

	float disperse_angle = 5.f;
	SoundDisperseTraits shoot_sound_traits{ 150.f, 500.f };

public:
	using Gun::Gun;
	size_t bullet() const override;
	rn::Vec2f getTrajectory() const override;
	Gun *copy() const override;
	void onShoot() override;

	std::chrono::milliseconds getRollback() const override;
};
