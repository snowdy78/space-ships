#pragma once
#include "decl.hpp"


struct SoundDisperseTraits
{
	float clear_sound_distance = 0.f, disperse_radius = 1000.f;
	SoundDisperseTraits(float clear_distance, float disperse_radius)
		: clear_sound_distance(clear_distance),
		  disperse_radius(disperse_radius)
	{}
};

class SoundDisperseEntity : public sf::Sound, public rn::LogicalObject
{
	float m_lerped_pure_sound_value, m_disperse_radius = 50.f;
	using Sound::setVolume;
	rn::Vec3f position{};

public:
	explicit SoundDisperseEntity(
		float lerped_pure_sound_value = 0.f, float disperse_radius = 1000.f, const sf::SoundBuffer *buffer = nullptr
	);
	explicit SoundDisperseEntity(
		SoundDisperseTraits traits, const sf::SoundBuffer *buffer = nullptr
	);

	const rn::Vec3f &getPosition() const;
	void setPosition(const rn::Vec3f &position);
	void setPureSoundLerpedValue(float lerped_value);
	void setDisperseRadius(float radius);
	float getDisperseRadius() const;
	float getPureSoundLerpedValue() const;
	float getVolumeInPoint(const rn::Vec3f &point) const;

	virtual void onPlay() {}
	virtual void onStop() {}
	virtual void onPause() {}
	void play() override;
	void stop() override;
	void pause() override;
	/// Update the volume of the sound based on the relative distance from the listener
	void update() override;
};
