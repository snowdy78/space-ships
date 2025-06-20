#include "game/AbstractBullet.hpp"

#include "Helpers.hpp"
#include "game/ships/AbstractShip.hpp"
#include "game/AbstractWeapon.hpp"
#include "game/GameManager.hpp"
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
		   && (author_ptr.expired() || !author_ptr.lock()->hasSameOwner(collidable));
}
const sf::Sprite &AbstractBullet::getSprite() const
{
	return sprite;
}
void AbstractBullet::onCollisionEnter(const Collidable* obstacle)
{
	if (auto hittable = dynamic_cast<const Hittable *>(obstacle);
		GameManager::exist() && existOnField() && hittable->existOnField())
	{
		GameManager::session()->action_manager.emplaceToTop<DestroySpaceFieldObjectAction>(TransferableActionProps{
			self() });
		GameManager::session()->action_manager.emplaceToTop<DealDamageAction>(TransferableActionProps{
			self(), hittable->self() });
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
	watcher.setViewArea({ GameManager::session()->camera.getPosition(), GameManager::session()->camera.getViewSize() });
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
		if (m_out_of_view_clock.is_stopped())
			m_out_of_view_clock.start();
	}
	else if (!m_out_of_view_clock.is_stopped())
	{
		m_out_of_view_clock.stop();
		m_out_of_view_clock.reset();
	}
	if (everyTime(m_out_of_view_clock, std::chrono::milliseconds(*props::destroy_after)))
	{
		destroy();
		m_out_of_view_clock.stop();
		m_out_of_view_clock.reset();
	}
}
