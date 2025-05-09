#include "game/guns/Pistol.hpp"
#include "SoundDisperseEntity.hpp"
#include "game/AbstractShip.hpp"
#include "game/GameGlobals.hpp"
#include "game/bullets/BaseBullet.hpp"

Pistol::Pistol(const AbstractShip *ship)
	: Gun(ship)
{}
Bullet *Pistol::createBullet() const
{
	auto bullet = new BaseBullet(this);
	bullet->setPosition(getPosition());
	return bullet;
}
Gun *Pistol::copy() const
{
	return new Pistol(user);
}
void Pistol::onShoot()
{
	if (GameGlobals::exist())
	{
		SoundDisperseEntity sound{ shoot_sound_traits, sound_buffer };
		GameGlobals::instance().sound_manager.emplace_back<SoundDisperseEntity>(
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
