#include "SoundDisperseEntity.hpp"

SoundDisperseEntity::SoundDisperseEntity(
	float clear_sound_distance, float disperse_force, const sf::SoundBuffer *buffer
)
	: clear_dist(clear_sound_distance),
	  disperse_radius(disperse_force)
{
	if (buffer)
	{
		setBuffer(*buffer);
	}
}
void SoundDisperseEntity::setClearSoundDistance(float distance)
{
	clear_dist = distance;
}
void SoundDisperseEntity::setDisperseRadius(float force)
{
	disperse_radius = force;
}

float SoundDisperseEntity::getClearSoundDistance() const
{
	return clear_dist;
}

float SoundDisperseEntity::getDisperseRadius() const
{
	return disperse_radius;
}

void SoundDisperseEntity::play()
{
	sf::Sound::play();
	onPlay();
}
void SoundDisperseEntity::stop()
{
	Sound::stop();
	onStop();
}
void SoundDisperseEntity::pause()
{
	Sound::pause();
	onPause();
}
void SoundDisperseEntity::update()
{
	using namespace rn::math;

	rn::Vec3f pdist = sf::Listener::getPosition() - getPosition();
	float dist		= length(pdist);
	float volume;
	if (dist < 0.00001f)
		volume = 100;
	else
		volume = (1.0f - std::clamp((clear_dist + dist) / disperse_radius, 0.f, 1.f)) * 100.f;
	setVolume(volume);
}
const rn::Vec3f &SoundDisperseEntity::getPosition() const
{
	return position;
}
