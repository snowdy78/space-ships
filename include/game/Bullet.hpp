#pragma once

#include "Collidable.hpp"
#include "DamageDealer.hpp"
#include "SoundDisperseEntity.hpp"
#include "SpaceFieldObject.hpp"
#include "colliders/EllipseCollider.hpp"
#include "components/FileLoader.hpp"
#include "decl.hpp"

class Bullet : public rn::MonoBehaviour, public Collidable, public DamageDealer, public SpaceFieldObject
{
	sf::Sprite sprite;

	float mass		   = 0.100f;
	float velocity	   = 15.f;
	float acceleration = 0.1f / 250.f;
	rn::Vec2f direction{};
	EllipseCollider collider;
	std::optional<rn::Vec2f> size; // defined when texture is setted

	void setTexture(const sf::Texture &texture);

protected:
	void updateCollider();
	virtual const sf::Texture &initTexture() const = 0;

public:
	const sf::Texture &getTexture() const;
	const Gun *author = nullptr;
	Bullet();
	~Bullet() override = 0;
	void setDirection(const rn::Vec2f &direction);
	void setMass(float mass);
	void setVelocity(float velocity);
	void setAcceleration(float acceleration);
	virtual void onMove();
	void setPosition(float x, float y);
	void setPosition(const rn::Vec2f &vector);
	void move(float x, float y);
	void move(const rn::Vec2f &p);

	const rn::Vec2f &getDirection() const;
	const sf::Sprite &getSprite() const;
	float getMass() const;
	float getVelocity() const;
	float getAcceleration() const;
	const std::optional<rn::Vec2f> &getSize() const;

	void onCollisionEnter(Collidable *obstacle) override;
	void destroyFromField() override;

	bool isIntersected(const rn::Vec2f &point) const override;
	const Collider *getCollider() const override;
	/**
	 * \brief Resolves a collision between this Bullet and the given Collidable.
	 *
	 * The collision is resolved if the Collidable is not a Bullet.
	 *
	 * \param collidable The collidable to check against.
	 * \return True if the collision is resolved, false otherwise.
	 */
	bool resolve(const Collidable *collidable) const override;
	void start() override;
	void update() override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
