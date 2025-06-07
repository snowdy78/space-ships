#include "game/bullets/BaseBullet.hpp"
#include "game/SpaceField.hpp"

const sf::Texture &BaseBullet::initTexture() const
{
	return *texture;
}
void BaseBullet::start()
{
	setDamage(initial_damage);

	float mass		   = 0.100f;
	float velocity	   = 15.f;
	float acceleration = 1.f / 2500.f;

	setVelocity(velocity);
	setMass(mass);
	setAcceleration(acceleration);
	AbstractBullet::start();
	fly_sound.start();
}
void BaseBullet::update()
{
	AbstractBullet::update();
	fly_sound.update();
}

void BaseBullet::onMove()
{
	AbstractBullet::onMove();
	fly_sound.setPosition({getPosition().x, getPosition().y, 0.f});
}

void BaseBullet::summonCopy(SpaceField &field) const
{
	field.summonBullet<BaseBullet>([this](BaseBullet &bullet) {
		bullet.author = author;
		bullet.setDirection(getDirection());
	});
}

void bullet_sound::start()
{
	setRelativeToListener(true);
    setLoop(true);
    play();
}
