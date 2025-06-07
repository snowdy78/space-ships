#pragma once

#include "Collidable.hpp"
#include "Hittable.hpp"
#include "RigitBody2d.hpp"
#include "SoundDisperseEntity.hpp"
#include "components/AnimatedSprite.hpp"
#include "components/FileLoader.hpp"
#include "decl.hpp"
#include "game/AbstractWeapon.hpp"
#include "game/colliders/EllipseCollider.hpp"
#include "components/effects/ShipEngineFlame.hpp"

template<class T>
concept GunConcept = std::is_base_of_v<AbstractWeapon, T> && !std::is_same_v<AbstractWeapon, T>;

class AbstractShip : public RigitBody2d, public Collidable, public Hittable
{
	friend class SpaceField;
	friend class AbstractWeapon;

protected:
	void updateGunPosition();
	void updateCollider();
	static size_t generateTeamHash(const sf::String &name);
	void setMoveDirection(const rn::Vec2f &move_dir);

public:
	AbstractShip(const sf::Texture &texture);

	const rn::Vec2f &getMoveDirection() const;
	bool isFriend(const AbstractShip *) const;
	void setTeam(const sf::String &team_name);
	void resetTeamName();
	const size_t &getTeam() const;
	void setGun(const AbstractWeapon &gun);
	const AbstractWeapon &getGun() const;
	rn::Vec2f getSize() const;
	/**
	 * \brief Shoot a bullet in the ship direction.
	 *
	 * A Gun will create a Bullet and shot in the ship direction. The direction
	 * is determined by the ship's rotation.
	 */
	void shoot();
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;
	void onDestroy() override;
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	const sf::Sprite &getSprite() const;
	const Collider *getCollider() const override;
	template<GunConcept T>
	void setGun();
	void onRotation() override;
	void onMove() override;
	void onHit() override;
	void onCollisionEnter(Collidable *collidable) override;
	bool resolve(const Collidable *collidable) const override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	inline static loading<AnimatedSprite> destroy_animation
		= FileLoader::Instance().addAnimatedSpriteToUpload("./img/animation/Explosion4", ".png").get();
	inline static loading<sf::SoundBuffer> hit_buffer = FileLoader::Instance().addSoundToUpload("hit.ogg").get();
	inline static loading<sf::SoundBuffer> destroy_buffer{
		FileLoader::Instance().addSoundToUpload("./assets/explosion.wav").get()
	};
	sf::Sprite sprite;
	std::unique_ptr<AbstractWeapon> gun;

private:
	ShipEngineFlame m_engine_effect;
	EllipseCollider collider;

	SoundDisperseTraits hit_sound_traits{ 100, 300 };
	SoundDisperseTraits destroy_sound_traits{ 200, 1000 };

	size_t m_team_hash = 0;
	rn::Vec2f m_move_dir{};
};

template<GunConcept GunT>
void AbstractShip::setGun()
{
	AbstractWeapon *gun_  = new GunT;
	gun_->user = this;
	gun.reset(gun_);
}
