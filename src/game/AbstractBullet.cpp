#include "game/AbstractBullet.hpp"
#include "game/AbstractShip.hpp"
#include "game/GameManager.hpp"
#include "game/Gun.hpp"
#include "game/actions/DealDamageAction.hpp"
#include "game/actions/DestroySpaceFieldObjectAction.hpp"


/**
 * @brief Default constructor for Bullet
 *
 * Construct a Bullet object with the image set to the contents of Bullet::texture
 */
AbstractBullet::AbstractBullet() = default;

AbstractBullet::~AbstractBullet() = default;

void AbstractBullet::start()
{
	RigitBody2d::start();
	setTexture(initTexture());
	updateCollider();
	setOrigin(*size / 2.f);
}

const std::optional<rn::Vec2f> &AbstractBullet::getSize() const
{
	return size;
}

void AbstractBullet::movement()
{
	RigitBody2d::movement();
	move(countMove());
}
void AbstractBullet::updateCollider()
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
const Collider *AbstractBullet::getCollider() const
{
	return &collider;
}

bool AbstractBullet::resolve(const Collidable *collidable) const
{
	return dynamic_cast<const Hittable *>(collidable)
		   && (!author || dynamic_cast<const AbstractShip *>(collidable) != author->user);
}
const sf::Sprite &AbstractBullet::getSprite() const
{
	return sprite;
}
void AbstractBullet::onCollisionEnter(Collidable *obstacle)
{
	if (auto hittable = dynamic_cast<Hittable *>(obstacle); GameManager::exist())
	{
		GameManager::session()->action_manager.emplaceToTop<DestroySpaceFieldObjectAction>(TransferableActionProps{this});
		GameManager::session()->action_manager.emplaceToTop<DealDamageAction>(TransferableActionProps{this, hittable});
	}
}
void AbstractBullet::setTexture(const sf::Texture &texture)
{
	sprite.setTexture(texture);
	size = rn::Vec2f{ texture.getSize() };
}

bool AbstractBullet::checkOutside()
{
	if (!GameManager::exist() || !getSize().has_value())
		return false;
	watcher.setViewArea(GameManager::session()->camera.getView().getViewport());
	return watcher.isOutOfViewArea(getTransform().transformRect({ {}, *getSize() }));
}

const sf::Texture &AbstractBullet::getTexture() const
{
	return *sprite.getTexture();
}

void AbstractBullet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform = getTransform();
	target.draw(sprite, states);
}
void AbstractBullet::onMove()
{
	updateCollider();
	if (checkOutside())
	{
		destroy();
	}
}
