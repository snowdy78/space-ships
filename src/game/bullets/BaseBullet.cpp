#include "game/bullets/BaseBullet.hpp"
#include "game/SpaceField.hpp"

const sf::Texture &BaseBullet::initTexture() const
{
	return *texture;
}
void BaseBullet::start()
{
	setDamage(initial_damage);
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

void BaseBullet::summonCopy(SpaceField &field) const
{
	field.summonBullet<BaseBullet>([this](BaseBullet &bullet) {
		bullet.setDirection(getDirection());
	}, author);
}

void bullet_sound::start()
{
	setRelativeToListener(true);
    setLoop(true);
    play();
}
