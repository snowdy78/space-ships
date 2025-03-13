#include "game/AbstractShip.hpp"
#include "game/Gun.hpp"
#include "game/RigitBody2d.hpp"
#include "game/SpaceField.hpp"
#include "game/guns/Pistol.hpp"

AbstractShip::AbstractShip(const sf::Texture &texture)
	: sprite(texture),
	  gun(new Pistol(this))
{
	hit_sound.setBuffer(*hit_buffer);
}
rn::Vec2f AbstractShip::getSize() const
{
	return rn::Vec2f(sprite.getTexture()->getSize());
}
const Gun *AbstractShip::getGun() const
{
	return gun.get();
}
void AbstractShip::setGun(const Gun &gun)
{
	this->gun.reset(gun.copy());
	updateGunPosition();
}
void AbstractShip::shoot()
{
	if (!gun)
		return;
	gun->shoot(getDirection());
}
sf::FloatRect AbstractShip::getLocalBounds() const
{
	return { {}, getSize() };
}
sf::FloatRect AbstractShip::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void AbstractShip::start()
{
	RigitBody2d::start();
	setOrigin(getSize() / 2.f);
	if (gun)
		gun->start();
	onMove();
}

void AbstractShip::update()
{
	RigitBody2d::update();
	if (gun)
		gun->update();
}

void AbstractShip::onEvent(sf::Event &event)
{
	RigitBody2d::onEvent(event);
	if (gun)
	{
		gun->onEvent(event);
	}

	if (rn::isKeydown(sf::Keyboard::LShift))
	{
		setVelocity(accelerated);
	}
	else if (rn::isKeyup(sf::Keyboard::LShift))
	{
		setVelocity(accelerated);
	}
}
void AbstractShip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	sf::RenderStates st = states;
	states.transform *= getTransform();
	target.draw(sprite, states);
	target.draw(health_bar, st);
}
void AbstractShip::onMove()
{
	updateGunPosition();
	updateCollider();
	health_bar.setPosition(getPosition() - getOrigin() + rn::Vec2f{ 0, getSize().y });
}
void AbstractShip::onRotation()
{
	updateCollider();
}
const sf::Sprite &AbstractShip::getSprite() const
{
	return sprite;
}
void AbstractShip::updateGunPosition()
{
	if (gun)
		gun->setPosition(getPosition());
}
const Collider *AbstractShip::getCollider() const
{
	return &collider;
}
void AbstractShip::updateCollider()
{
	rn::Circle circle{ getSize().x / 2.f };
	circle.setPosition(getPosition());
	circle.setOrigin(getOrigin());
	circle.setRotation(getRotation());
	circle.setScale(getScale());
	collider.transform(rn::math::ellipse(circle));
}
bool AbstractShip::resolve(const Collidable *collidable) const
{
	auto bullet = dynamic_cast<const Bullet *>(collidable);
	bool state	= bullet;
	state		= state && bullet->gun->user != this;
	return state;
}
void AbstractShip::onCollisionEnter(Collidable *collidable)
{
	if (auto dd = dynamic_cast<const DamageDealer *>(collidable))
	{
		takeDamage(dd->getDamage());
	}
}
void AbstractShip::onHit()
{
	Hittable::onHit();
	hit_sound.play();
	if (getHealth() <= 0 && field)
	{
		beforeDie();
		field->remove(this);
	}
}
bool AbstractShip::isDead() const
{
	return is_dead;
}
