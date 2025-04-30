#include "game/Bullet.hpp"
#include "game/AbstractShip.hpp"
#include "game/Gun.hpp"
#include "game/SpaceField.hpp"

/**
 * \brief Default constructor for Bullet
 *
 * Construct a Bullet object with the image set to the contents of Bullet::texture
 */
Bullet::Bullet(const Gun *gun)
	: author(gun)
{
	setTexture(*texture);
	setOrigin(rn::Vec2f{ texture->getSize() / 2u });
	updateCollider();
	fly_sound.start();
}

Bullet::~Bullet() {}

void Bullet::update()
{
	velocity += acceleration;
	acceleration *= 0.99f;
	move(direction * velocity);
	fly_sound.update();
}
rn::Vec2f Bullet::getSize() const
{
	return rn::Vec2f{ sprite.getTexture()->getSize() };
}
void Bullet::setDirection(const rn::Vec2f &direction)
{
	this->direction = rn::math::norm(direction);
}
void Bullet::setMass(float mass)
{
	this->mass = mass;
}
void Bullet::setVelocity(float velocity)
{
	this->velocity = velocity;
}
void Bullet::setAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}
const rn::Vec2f &Bullet::getDirection() const
{
	return direction;
}
float Bullet::getMass() const
{
	return mass;
}
float Bullet::getVelocity() const
{
	return velocity;
}
float Bullet::getAcceleration() const
{
	return acceleration;
}
bool Bullet::isIntersected(const rn::Vec2f &point) const
{
	return collider.getColliderShape().contains(point);
}
void Bullet::setPosition(float x, float y)
{
	sf::Transformable::setPosition(x, y);
	onMove();
}
void Bullet::setPosition(const rn::Vec2f &vector)
{
	sf::Transformable::setPosition(vector);
	onMove();
}
void Bullet::move(float x, float y)
{
	sf::Transformable::move(x, y);
	onMove();
}
void Bullet::move(const rn::Vec2f &p)
{
	sf::Transformable::move(p);
	onMove();
}

void Bullet::updateCollider()
{
	rn::Circle circle(getSize().x / 2.f);
	circle.setScale(getScale());
	circle.setOrigin(getOrigin());
	circle.setPosition(getPosition());
	circle.setRotation(getRotation());
	collider.transform(circle);
}
const Collider *Bullet::getCollider() const
{
	return &collider;
}

bool Bullet::resolve(const Collidable *collidable) const
{
	return dynamic_cast<const Hittable *>(collidable)
		   && (!gun || dynamic_cast<const AbstractShip *>(collidable) != gun->user);
}
const sf::Sprite &Bullet::getSprite() const
{
	return sprite;
}
void Bullet::destroyFromField() const
{
	if (field)
	{
		beforeDestroy();
		field->destroyBullet(this);
	}
}
void Bullet::onCollisionEnter(Collidable *obstacle)
{
	if (auto hittable = dynamic_cast<Hittable *>(obstacle))
	{
		destroyFromField();
	}
}
void Bullet::setTexture(const sf::Texture &texture)
{
	sprite.setTexture(texture);
}
const sf::Texture &Bullet::getTexture() const
{
	return *sprite.getTexture();
}

void Bullet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform = getTransform();
	target.draw(sprite, states);
}
void Bullet::summonCopy(SpaceField *field) const 
{
	field->summonBullet(copy(), getDirection());
}
void Bullet::onMove() 
{
	updateCollider();
	//fly_sound.setPosition({getPosition().x, getPosition().y, 0});
}

void Bullet::bullet_sound::start() 
{
	setLoop(true);
	play();
}
