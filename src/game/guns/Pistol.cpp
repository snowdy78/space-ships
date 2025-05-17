#include "game/guns/Pistol.hpp"
#include "SoundDisperseEntity.hpp"
#include "game/AbstractShip.hpp"
#include "game/GameManager.hpp"
#include "game/bullets/BaseBullet.hpp"

Pistol::Pistol(const AbstractShip *_ship)
	: Gun(_ship)
{}
void Pistol::summonBullet() const
{
	if (GameManager::exist())
	{
		GameManager::instance().field.summonBullet<BaseBullet>([this](BaseBullet &bullet) {
			bullet.setPosition(getPosition());
			bullet.setDirection(getTrajectory());
		}, this);
	}
}
Gun *Pistol::copy() const
{
	return new Pistol(user);
}
void Pistol::onShoot()
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
float Pistol::getMillisDelay() const
{
	return 500;
}
rn::Vec2f Pistol::getTrajectory() const
{
	auto angle
		= rn::math::rot(ship->getDirection()) + rn::math::degrees(rn::random::real(-1.f, 1.f) * disperse_angle / 2.f);
	return rn::math::direction(angle);
}
