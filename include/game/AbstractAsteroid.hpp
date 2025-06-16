#pragma once

#include "Collidable.hpp"
#include "ObjectWatcher.hpp"
#include "RigitBody2d.hpp"
#include "colliders/PolygonCollider.hpp"
#include "decl.hpp"
#include "components/GameConfiguration.hpp"

class AbstractAsteroid : public RigitBody2d, public Collidable
{
	struct props
	{
		inline static auto &self_config = config::instance().get("AbstractAsteroid");
		G_CONFIG_PROP_DEFINE(self_config, destroy_after);
	};
public:
	AbstractAsteroid(const sf::Texture &texture);
	~AbstractAsteroid() override;

	rn::Vec2f getSize() const;
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	void movement() override;
	void onMove() override;
	bool checkOutside();
	void rotation() override;
	void start() override;
	void update() override;
	virtual void updateCollider() = 0;
	const Collider *getCollider() const override;
	void onCollisionEnter(Collidable *obstacle) override;
	bool resolve(const Collidable *obstacle) const override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	
protected:
	PolygonCollider collider;
	float m_density = 1.0f;

private:
	sf::Sprite m_sprite;
	rn::Stopwatch m_out_of_view_clock;
	rn::Animation m_transition{ this };
	ObjectWatcher m_watcher;
};
