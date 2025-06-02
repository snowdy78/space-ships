#pragma once

#include "Collidable.hpp"
#include "RigitBody2d.hpp"
#include "SpaceFieldObject.hpp"
#include "colliders/PolygonCollider.hpp"
#include "decl.hpp"

class AbstractAsteroid : public RigitBody2d, public SpaceFieldObject, public Collidable
{
public:
	AbstractAsteroid(const sf::Texture &texture);
	~AbstractAsteroid() override;

	rn::Vec2f getSize() const;
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	void movement() override;
	void onMove() override;
	void rotation() override;
	void start() override;
	void update() override;
	virtual void updateCollider() = 0;
	const Collider *getCollider() const override;
	void onCollisionEnter(Collidable *obstacle) override;
	bool resolve(const Collidable *obstacle) const override;
	rn::Vec2f countMove() const override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	PolygonCollider collider;
	float m_density = 1.0f;

private:
	sf::Sprite m_sprite;
	rn::Animation m_transition{ this };
};
