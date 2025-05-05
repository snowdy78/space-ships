#pragma once

#include "Collidable.hpp"
#include "Hittable.hpp"
#include "RigitBody2d.hpp"
#include "SoundDisperseEntity.hpp"
#include "SpaceFieldObject.hpp"
#include "components/AnimatedSprite.hpp"
#include "components/FileLoader.hpp"
#include "decl.hpp"
#include "game/Gun.hpp"
#include "game/colliders/EllipseCollider.hpp"


template<class T>
concept GunConcept = std::is_base_of<Gun, T>::value && !std::is_same_v<Gun, T>;

class AbstractShip : public RigitBody2d, public Collidable, public Hittable, public SpaceFieldObject
{
	friend class SpaceField;
	friend class Gun;

protected:
	void updateGunPosition();
	void updateCollider();
	static sf::String generateRandomTeamName(size_t length = 16);
public:
	AbstractShip(const sf::Texture &texture);

	bool isFriend(const AbstractShip *);
	void setTeamName(const sf::String &team_name);
	void resetTeamName();
	const sf::String &getTeamName() const;
	void setGun(const Gun &gun);
	const Gun *getGun() const;
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
	void setAcceleration(float acceleration);
	float getAcceleration() const;
	virtual void onAcceleration() {}
	void onRotation() override;
	void onMove() override;
	void onHit() override;
	void onCollisionEnter(Collidable *collidable) override;
	bool resolve(const Collidable *collidable) const override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void destroyFromField() override;
	virtual AbstractShip *copy() const = 0;
protected:

	std::unique_ptr<Gun> gun;
	sf::Sprite sprite;
	EllipseCollider collider;

	inline static loading<AnimatedSprite> destroy_animation
		= FileLoader::Instance().addAnimatedSpriteToUpload("./img/animation/Explosion4", ".png").get();
	inline static loading<sf::SoundBuffer> hit_buffer = FileLoader::Instance().addSoundToUpload("hit.ogg").get();
	inline static loading<sf::SoundBuffer> destroy_buffer{FileLoader::Instance().addSoundToUpload("./assets/explosion.wav").get()};
	SoundDisperseTraits hit_sound_traits{ 100, 300 };
	SoundDisperseTraits destroy_sound_traits{ 200, 1000};	

	sf::String team_name = generateRandomTeamName();
	float m_acceleration{1};
};

template<GunConcept T>
void AbstractShip::setGun()
{
	gun.reset(new T(this));
}
