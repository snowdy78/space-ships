#include "game/AbstractShip.hpp"
#include "SoundDisperseEntity.hpp"
#include "components/AnimatedSprite.hpp"
#include "components/EffectManager.hpp"
#include "game/GameGlobals.hpp"
#include "game/Gun.hpp"
#include "game/RigitBody2d.hpp"
#include "game/SpaceField.hpp"
#include "game/guns/Pistol.hpp"


AbstractShip::AbstractShip(const sf::Texture &texture)
	: sprite(texture),
	  gun(new Pistol(this))
{}
bool AbstractShip::friendly() const
{
	return is_friendly;
}
void AbstractShip::friendly(bool friendly)
{
	is_friendly = friendly;
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
void AbstractShip::destroyFromField() const
{
	if (field)
	{
		field->remove(this);
	}
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
	if (GameGlobals::exist())
	{
		GameGlobals::instance().sound_manager.emplace_back<SoundDisperseEntity>(
			[this](auto sound) {
				sound->setPosition(rn::Vec3f(getPosition().x, getPosition().y, 0.f));
				
			},
			hit_sound_traits,
			hit_buffer
		);
	}
	if (getHealth() <= 0 && field)
	{
		is_dead = true;
		onDeath();
		field->remove(this);
	}
}
bool AbstractShip::isDead() const
{
	return is_dead;
}

void AbstractShip::onDeath()
{
	if (GameGlobals::exist())
	{
		GameGlobals::instance().effect_manager.emplace_back<AnimatedSprite>(
			[this](auto effect) {
				effect->setOrigin(getSize() / 2.f);
				effect->setPosition(getPosition());
				effect->setDuration(std::chrono::milliseconds(750));
			},
			*destroy_animation
		);
		GameGlobals::instance().sound_manager.emplace_back<SoundDisperseEntity>(
			[this](auto sound) {
				sound->setPosition(rn::Vec3f(getPosition().x, getPosition().y, 0.f));
			},
			destroy_sound_traits,
			destroy_buffer
		);
	}
}
