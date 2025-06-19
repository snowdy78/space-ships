#include "SoundDisperseEntity.hpp"
#include "game/settings/AudioSettings.hpp"

SoundDisperseEntity::SoundDisperseEntity(
	float lerped_pure_sound_radius, float disperse_radius, const sf::SoundBuffer *buffer
)
	: m_lerped_pure_sound_value(std::clamp(disperse_radius * lerped_pure_sound_radius, 0.f, disperse_radius)),
	  m_disperse_radius(disperse_radius)
{
	setRelativeToListener(true);
	if (buffer)
		setBuffer(*buffer);
}
SoundDisperseEntity::SoundDisperseEntity(SoundDisperseTraits traits, const sf::SoundBuffer *buffer)
	: m_lerped_pure_sound_value(traits.clear_sound_distance),
	  m_disperse_radius(traits.disperse_radius)
{
	if (buffer)
		setBuffer(*buffer);
}
void SoundDisperseEntity::setPureSoundLerpedValue(float lerped_value)
{
	m_lerped_pure_sound_value = std::clamp(lerped_value, 0.f, m_disperse_radius);
}
void SoundDisperseEntity::setDisperseRadius(float radius)
{
	m_disperse_radius = radius;
	m_lerped_pure_sound_value = std::clamp(m_lerped_pure_sound_value, 0.f, m_disperse_radius);
}

float SoundDisperseEntity::getPureSoundLerpedValue() const
{
	return m_lerped_pure_sound_value;
}

float SoundDisperseEntity::getVolumeInPoint(const rn::Vec3f &point) const
{
	using rn::math::length;
	auto dist = point - getPosition();
	float l	  = length(dist);
	if (m_disperse_radius == 0.f || l > m_disperse_radius)
	{
		return 0.f;
	}
	float pure_sound_radius = m_lerped_pure_sound_value * m_disperse_radius;
	if (l <= pure_sound_radius)
	{
		return sfx_volume_percentage();
	}
	float r = m_disperse_radius - pure_sound_radius;
	float rl = l - pure_sound_radius;
	return (1.0f - rl / r) * sfx_volume_percentage();
}

float SoundDisperseEntity::getDisperseRadius() const
{
	return m_disperse_radius;
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
	setVolume(getVolumeInPoint(sf::Listener::getPosition()));
}
const rn::Vec3f &SoundDisperseEntity::getPosition() const
{
	return position;
}

void SoundDisperseEntity::setPosition(const rn::Vec3f &position)
{
	this->position = position;
}
