#pragma once
#include "decl.hpp"


struct SoundDisperseTraits
{
	float clear_sound_distance = 0.f, disperse_force = 1000.f;
	SoundDisperseTraits(float clear_distance, float disperse_force)
		: clear_sound_distance(clear_distance),
		  disperse_force(disperse_force)
	{}
};

class SoundDisperseEntity : public sf::Sound, public rn::LogicalObject
{
	float clear_dist, disperse_radius = 50.f;
	using Sound::setVolume;
	rn::Vec3f position{};

public:
	explicit SoundDisperseEntity(
		float clear_sound_distance = 0.f, float disperse_force = 1000.f, const sf::SoundBuffer *buffer = nullptr
	);
	explicit SoundDisperseEntity(
		SoundDisperseTraits traits, const sf::SoundBuffer *buffer = nullptr
	);

	const rn::Vec3f &getPosition() const;
	void setPosition(const rn::Vec3f &position);
	void setClearSoundDistance(float distance);
	void setDisperseRadius(float force);
	float getDisperseRadius() const;
	float getClearSoundDistance() const;

	virtual void onPlay() {}
	virtual void onStop() {}
	virtual void onPause() {}
	void play() override;
	void stop() override;
	void pause() override;
	/// Update the volume of the sound based on the relative distance from the listener
	void update() override;
};
