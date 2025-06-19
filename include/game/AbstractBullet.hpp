#pragma once

#include "Collidable.hpp"
#include "DamageDealer.hpp"
#include "RigitBody2d.hpp"
#include "colliders/EllipseCollider.hpp"
#include "decl.hpp"
#include <optional>
#include "ObjectWatcher.hpp"
#include "SpaceField.hpp"
#include "components/GameConfiguration.hpp"

class AbstractBullet : public RigitBody2d, public Collidable, public DamageDealer
{
	struct props
	{
		inline static auto &self_config = config::instance().get("AbstractBullet");
		G_CONFIG_PROP_DEFINE(self_config, destroy_after);
	};
	sf::Sprite sprite;

	ObjectWatcher watcher;
	rn::Stopwatch m_out_of_view_clock;
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
	void onCollisionEnter(const Collidable* obstacle) override;
	const Collider *getCollider() const override;

	bool resolve(const Collidable *collidable) const override;
	void start() override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
