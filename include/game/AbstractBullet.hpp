#pragma once

#include "Collidable.hpp"
#include "DamageDealer.hpp"
#include "RigitBody2d.hpp"
#include "colliders/EllipseCollider.hpp"
#include "decl.hpp"
#include <optional>
#include "ObjectWatcher.hpp"
#include "SpaceField.hpp"

class AbstractBullet : public RigitBody2d, public Collidable, public DamageDealer
{
	sf::Sprite sprite;

	ObjectWatcher watcher;
	rn::Stopwatch out_of_view_timer;
	// TODO destroy after placed in config
	constexpr static std::chrono::milliseconds destroy_after{1000};
	EllipseCollider collider;
	std::optional<rn::Vec2f> size; // defined when texture is set

	void setTexture(const sf::Texture &texture);
	bool checkOutside();

protected:
	void updateCollider();
	virtual const sf::Texture &initTexture() const = 0;

public:
	const sf::Texture &getTexture() const;
	SpaceField::StatePtr<AbstractWeapon> author_ptr;
	AbstractBullet();
	~AbstractBullet() override = 0;

	const sf::Sprite &getSprite() const;
	const std::optional<rn::Vec2f> &getSize() const;
	void movement() override;
	void onMove() override;
	void onCollisionEnter(Collidable *obstacle) override;
	const Collider *getCollider() const override;

	bool resolve(const Collidable *collidable) const override;
	void start() override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
