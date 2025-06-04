#include "game/guns/Pistol.hpp"
#include "SoundDisperseEntity.hpp"
#include "game/AbstractShip.hpp"
#include "game/GameManager.hpp"
#include "game/bullets/BaseBullet.hpp"

size_t Pistol::bullet() const
{
	return BaseBullet::identifier;
}
Gun *Pistol::copy() const
{
	return new Pistol;
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
std::chrono::milliseconds Pistol::getRollback() const
{
	return std::chrono::milliseconds{500};
}
rn::Vec2f Pistol::getTrajectory() const
{
	auto angle
		= rn::math::rot(user->getDirection()) + rn::math::degrees(rn::random::real(-1.f, 1.f) * disperse_angle / 2.f);
	return direction(angle);
}
