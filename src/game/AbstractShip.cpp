#include "game/AbstractShip.hpp"
#include "SoundDisperseEntity.hpp"
#include "components/AnimatedSprite.hpp"
#include "components/EffectManager.hpp"
#include "game/GameManager.hpp"
#include "game/AbstractWeapon.hpp"
#include "game/RigitBody2d.hpp"
#include "game/actions/TakeDamageAction.hpp"
#include "game/guns/Pistol.hpp"
#include "game/DamageDealer.hpp"
#include "game/AbstractBullet.hpp"

AbstractShip::AbstractShip(const sf::Texture &texture)
	: sprite(texture)
{
}
bool AbstractShip::isFriend(const AbstractShip *other_ship) const
{
	return other_ship->getTeam() == getTeam();
}
void AbstractShip::setTeam(const sf::String &team_name)
{
	m_team_hash = generateTeamHash(team_name);
}
void AbstractShip::resetTeamName()
{
	m_team_hash = 0;
}
const size_t &AbstractShip::getTeam() const
{
	return m_team_hash;
}
rn::Vec2f AbstractShip::getSize() const
{
	return rn::Vec2f(sprite.getTexture()->getSize());
}
SpaceField::StatePtr<AbstractWeapon> AbstractShip::getGun() const
{
	return gun;
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

void AbstractShip::onMove()
{
	updateGunPosition();
	updateCollider();
	health_bar.setPosition(getPosition() - getOrigin() + rn::Vec2f{ 0, getSize().y });
	m_engine_effect.setPosition(getPosition());
}
void AbstractShip::onRotation()
{
	updateCollider();
	m_engine_effect.setRotation(getRotation() + 90);
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
	m_collider_widget.setRadius(getSize().x / 2.f);
	m_collider_widget.setFillColor({ 0, 210, 160, 64 });
	m_collider_widget.setPosition(getPosition());
	m_collider_widget.setOrigin(getOrigin());
	m_collider_widget.setRotation(getRotation());
	m_collider_widget.setScale(getScale());
	
	collider.transform(rn::math::ellipse(m_collider_widget));
}
size_t AbstractShip::generateTeamHash(const sf::String &name)
{
	return std::hash<const char *>()(name.toAnsiString().c_str());
}
bool AbstractShip::resolve(const Collidable *collidable) const
{
	auto bullet = dynamic_cast<const AbstractBullet *>(collidable);
	bool state	= bullet && (bullet->author_ptr.expired() || bullet->author_ptr.lock() != gun);
	return state;
}
void AbstractShip::onCollisionEnter(const Collidable* collidable)
{
	if (auto dd = dynamic_cast<const DamageDealer *>(collidable); GameManager::exist() && existOnField() && dd->existOnField())
		GameManager::session()->action_manager.emplaceToTop<TakeDamageAction>(TransferableActionProps{ self(),
																									   dd->self() });
}
void AbstractShip::onHit()
{
	Hittable::onHit();

	if (!GameManager::exist())
		return;
	GameManager::instance().sound_manager.emplace_back<SoundDisperseEntity>(
		[this](SoundDisperseEntity &sound) {
			sound.setPosition(rn::Vec3f(getPosition().x, getPosition().y, 0.f));
		},
		hit_sound_traits, hit_buffer
	);
}
void AbstractShip::onDestroy()
{
	if (GameManager::exist())
	{
		GameManager::instance().effect_manager.emplace_back<AnimatedSprite>(
			[this](AnimatedSprite &effect) {
				effect.setOrigin(getSize() / 2.f);
				effect.setPosition(getPosition());
				effect.setDuration(std::chrono::milliseconds(750));
			},
			*destroy_animation
		);
		GameManager::instance().sound_manager.emplace_back<SoundDisperseEntity>(
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

void AbstractShip::start()
{
	RigitBody2d::start();
	assignGameWeaponToPlayer<Pistol>(*this);
	setOrigin(getSize() / 2.f);
	m_engine_effect.setOrigin(m_engine_effect.getSize().x / 2.f, -getSize().y / 8.f);
	onMove();
	if (gun)
		gun->start();
	m_engine_effect.start();
}

void AbstractShip::update()
{
	RigitBody2d::update();
	if (gun)
		gun->update();
	m_engine_effect.update();
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
	target.draw(m_engine_effect);
	target.draw(sprite, states);
	try
	{
		if (static_cast<bool>(*collider_visible))
		{
			target.draw(m_collider_widget, st);
		}
	} catch (std::exception &err)
	{
		std::cerr << err.what() << ": AbstractShip collider_visible is not boolean ";
		throw;
	}
	target.draw(health_bar, st);
}
