#include "game/bullets/BaseBullet.hpp"

BaseBullet::BaseBullet(const Gun *gun)
	: Bullet(gun)
{
	setDamage(initial_damage);
}
const sf::Texture &BaseBullet::initTexture() const
{
	return *texture;
}
void BaseBullet::start()
{
	Bullet::start();
	fly_sound.start();
}
void BaseBullet::update()
{
	Bullet::update();
	fly_sound.update();
}
Bullet *BaseBullet::copy() const
{
	return new BaseBullet(gun);
}
