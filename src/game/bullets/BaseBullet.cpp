#include "game/bullets/BaseBullet.hpp"

BaseBullet::BaseBullet(const Gun *_gun)
	: Bullet(_gun)
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

void BaseBullet::onMove()
{
	Bullet::onMove();
	fly_sound.setPosition({getPosition().x, getPosition().y, 0.f});
}

Bullet *BaseBullet::copy() const
{
	return new BaseBullet(gun);
}

void bullet_sound::start()
{
	setRelativeToListener(true);
    setLoop(true);
    play();
}
