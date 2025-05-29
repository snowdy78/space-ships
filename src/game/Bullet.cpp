#include "game/Bullet.hpp"
#include "game/AbstractShip.hpp"
#include "game/GameManager.hpp"
#include "game/Gun.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/DealDamageAction.hpp"
#include "game/actions/DestroySpaceFieldObjectAction.hpp"


/**
 * @brief Default constructor for Bullet
 *
 * Construct a Bullet object with the image set to the contents of Bullet::texture
 */
Bullet::Bullet() = default;

Bullet::~Bullet() = default;

void Bullet::start()
{
	setTexture(initTexture());
	updateCollider();
	setOrigin(*size / 2.f);
}

void Bullet::update()
{
	velocity += acceleration;
	acceleration *= 0.99f;
	move(direction * velocity);
}
const std::optional<rn::Vec2f> &Bullet::getSize() const
{
	return size;
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
	if (size)
	{
		rn::Circle circle(size->x / 2.f);
		circle.setScale(getScale());
		circle.setOrigin(getOrigin());
		circle.setPosition(getPosition());
		circle.setRotation(getRotation());
		collider.transform(circle);
	}
}
const Collider *Bullet::getCollider() const
{
	return &collider;
}

bool Bullet::resolve(const Collidable *collidable) const
{
	return dynamic_cast<const Hittable *>(collidable)
		   && (!author || dynamic_cast<const AbstractShip *>(collidable) != author->user);
}
const sf::Sprite &Bullet::getSprite() const
{
	return sprite;
}
void Bullet::onCollisionEnter(Collidable *obstacle)
{
	if (auto hittable = dynamic_cast<Hittable *>(obstacle); GameManager::exist())
	{
		GameManager::instance().action_manager.emplaceToTop<DestroySpaceFieldObjectAction>(TransferableActionProps{this});
		GameManager::instance().action_manager.emplaceToTop<DealDamageAction>(TransferableActionProps{this, hittable});
	}
}
void Bullet::setTexture(const sf::Texture &texture)
{
	sprite.setTexture(texture);
	size = rn::Vec2f{ texture.getSize() };
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
void Bullet::onMove()
{
	updateCollider();
}
