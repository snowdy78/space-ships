#pragma once

#include "AbstractShip.hpp"
#include "BulletMother.hpp"
#include "decl.hpp"
#include "game/AbstractAsteroid.hpp"

template<class T>
concept SpaceFieldObjectConcept = std::is_base_of_v<GameObject, T> && !std::is_abstract_v<T>;
template<class T, class... Args>
concept ShipConcept = std::is_base_of_v<AbstractShip, T> && SpaceFieldObjectConcept<T> && requires(Args const &...args) { T(args...); };
template<class T>
concept BulletConcept = std::is_base_of_v<Bullet, T> && SpaceFieldObjectConcept<T>;
template<class T>
concept AsteroidConcept = std::is_base_of_v<AbstractAsteroid, T> && SpaceFieldObjectConcept<T>;

class SpaceField : public sf::Drawable, public rn::LogicalObject
{
public:
	template<class T>
	using container = std::vector<T>;
	using ship_ptr_t = std::unique_ptr<AbstractShip>;
	using asteroid_ptr_t = std::unique_ptr<AbstractAsteroid>;
	using bullet_ptr_t = Bullet *;
	using ships_container = container<ship_ptr_t>;
	using asteroids_container = container<asteroid_ptr_t>;
	using iterator		 = std::vector<AbstractShip *>::iterator;
	using const_iterator = std::vector<AbstractShip *>::const_iterator;

	SpaceField(const Camera2d *camera = nullptr);
	SpaceField(const SpaceField &field) = delete;
	~SpaceField() override = default;

	void setCamera(const Camera2d *camera2d);
	const Camera2d *getCamera() const;
	const BulletMother &getBulletMother() const;
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;

	AbstractShip *get(size_t index);
	const AbstractShip *get(size_t index) const;
	AbstractShip *operator[](size_t index);
	const AbstractShip *operator[](size_t index) const;
	const ships_container &getShips() const;

	size_t size() const;
	void clear();

	template<class T, class... Args>
		requires (ShipConcept<T, Args...>)
	AbstractShip *summonShip(const Args &...args) noexcept;
	template<BulletConcept BulletT>
	Bullet *summonBullet(const std::function<void(BulletT &)> &init, const Gun *gun) noexcept;
	template<AsteroidConcept AsteroidT>
	AbstractAsteroid *summonAsteroid(const rn::Vec2f &summon_position = {}, const rn::Vec2f &velocity = {});
	void push_back(AbstractShip *ship);
	void push_back(Bullet *bullet, const Gun *const &gun = nullptr);
	void push_back(AbstractAsteroid *asteroid, const rn::Vec2f &summon_position = {}, const rn::Vec2f &velocity = {});
	void destroyAsteroid(const AbstractAsteroid *asteroid);
	void destroyBullet(Bullet *const &bullet);
	void destroyShip(const AbstractShip *ship);
	virtual void onObjectAppend(GameObject *object) const
	{
	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	SpaceField &operator=(const SpaceField &other) = delete;
private:
	void garbageFree();

	ships_container m_ships{};
	asteroids_container m_asteroids{};

	const Camera2d *m_camera;
	BulletMother m_mother{ m_camera };
};


template<class T, class... Args>
	requires(ShipConcept<T, Args...>)
AbstractShip *SpaceField::summonShip(const Args &...args) noexcept
{
	T *ship = new T(args...);
	ship->start();
	m_ships.emplace_back(ship);
	this->onObjectAppend(ship);
	return ship;
}

template<BulletConcept BulletT>
Bullet *SpaceField::summonBullet(const std::function<void(BulletT &)> &init, const Gun *gun) noexcept
{
	BulletT *bullet = new BulletT;
	init(*bullet);
	bullet->author = gun;
	m_mother.summon(bullet);
	bullet->start();
	bullet->onSummon();
	this->onObjectAppend(bullet);
	return bullet;
}

template<AsteroidConcept AsteroidT>
AbstractAsteroid *SpaceField::summonAsteroid(const rn::Vec2f &summon_position, const rn::Vec2f &velocity)
{
	AsteroidT *asteroid = new AsteroidT;
	asteroid->setPosition(summon_position);
	asteroid->setVelocity(rn::math::length(velocity));
	asteroid->setDirection(rn::math::norm(velocity));
	asteroid->start();
	m_asteroids.emplace_back(asteroid);
	this->onObjectAppend(asteroid);
	return asteroid;
}
