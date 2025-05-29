#pragma once

#include <vector>
#include "Camera2d.hpp"
#include "decl.hpp"
#include "game/Bullet.hpp"
class ChildBullet : public rn::LogicalObject
{
	friend class BulletMother;

public:
	constexpr static float life_time_ms = 1000.f;

	std::unique_ptr<Bullet> bullet{ nullptr };
	bool need_to_remove = false;
	BulletMother *mother;
	rn::Stopwatch clock;

	ChildBullet(BulletMother *mother, Bullet *bullet);
	const Bullet *get() const;
	bool isOutsideViewArea() const;
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;

	bool operator==(const Bullet &bullet) const;
	bool operator!=(const Bullet &bullet) const;
};
class BulletMother : public sf::Transformable, public rn::LogicalObject, public std::vector<ChildBullet>
{
public:
	
	BulletMother(const Camera2d *camera = nullptr);
	void setCamera(const Camera2d *camera2d);
	const Camera2d *getCamera() const;
	sf::View getViewArea() const;
	void destroy(const Bullet *bullet);
	void summon(Bullet *bullet);
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;

private:
	/**
	 * @brief нужно для определения области существования пуль (чтобы они уничтожались, выходя за пределы видимости)
	 */
	const Camera2d *camera = nullptr;
};
