#include "game/AbstractShip.hpp"
#include "SoundDisperseEntity.hpp"
#include "components/AnimatedSprite.hpp"
#include "components/EffectManager.hpp"
#include "game/GameGlobals.hpp"
#include "game/Gun.hpp"
#include "game/RigitBody2d.hpp"
#include "game/actions/DestroyShipAction.hpp"
#include "game/actions/TakeDamageAction.hpp"
#include "game/guns/Pistol.hpp"


AbstractShip::AbstractShip(const sf::Texture &texture)
	: sprite(texture),
	  gun(new Pistol(this))
{
}
bool AbstractShip::isFriend(const AbstractShip *other_ship) const
{
	return other_ship->getTeamName() == getTeamName();
}
void AbstractShip::setTeamName(const sf::String &team_name)
{
	this->team_name = team_name;
}
void AbstractShip::resetTeamName()
{
	team_name = generateRandomTeamName();
}
const sf::String &AbstractShip::getTeamName() const
{
	return team_name;
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
	if (gun)
		gun->shoot();
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
		gun->onEvent(event);
}
void AbstractShip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	sf::RenderStates st = states;
	states.transform *= getTransform();
	target.draw(sprite, states);
	target.draw(health_bar, st);
}
void AbstractShip::destroyFromField()
{
	if (GameGlobals::exist())
		GameGlobals::instance().action_manager.emplaceToTop<DestroyShipAction>(TA_Props{this});
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
sf::String AbstractShip::generateRandomTeamName(size_t length)
{
	static const sf::String hash_limit{ "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	sf::String result;
	while (length != 0)
	{
		result += *rn::random::item(hash_limit.begin(), hash_limit.end());
		length--;
	}
	return result;
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
	if (auto dd = dynamic_cast<DamageDealer *>(collidable); GameGlobals::exist())
		GameGlobals::instance().action_manager.emplaceToTop<TakeDamageAction>(TA_Props{this, dd});
}
void AbstractShip::onHit()
{
	Hittable::onHit();

	if (!GameGlobals::exist())
		return;
	GameGlobals::instance().sound_manager.emplace_back<SoundDisperseEntity>(
		[this](SoundDisperseEntity &sound) {
			sound.setPosition(rn::Vec3f(getPosition().x, getPosition().y, 0.f));
		},
		hit_sound_traits, hit_buffer
	);
}
void AbstractShip::onDestroy()
{
	if (GameGlobals::exist())
	{
		GameGlobals::instance().effect_manager.emplace_back<AnimatedSprite>(
			[this](AnimatedSprite &effect) {
				effect.setOrigin(getSize() / 2.f);
				effect.setPosition(getPosition());
				effect.setDuration(std::chrono::milliseconds(750));
			},
			*destroy_animation
		);
		GameGlobals::instance().sound_manager.emplace_back<SoundDisperseEntity>(
			[this](SoundDisperseEntity &sound) {
				sound.setPosition(rn::Vec3f(getPosition().x, getPosition().y, 0.f));
			},
			destroy_sound_traits, destroy_buffer
		);
	}
}
void AbstractShip::setMoveDirection(const rn::Vec2f &move_dir)
{
	m_move_dir = move_dir;
	if (m_move_dir.x != m_move_dir.x || m_move_dir.y != m_move_dir.y)
		m_move_dir = {};
}
const rn::Vec2f &AbstractShip::getMoveDirection() const
{
	return m_move_dir;
}
