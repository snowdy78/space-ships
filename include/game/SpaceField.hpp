#pragma once

#include "Camera2d.hpp"
#include "decl.hpp"
#include "game/ObjectWatcher.hpp"
#include "SpaceFieldObject.hpp"

template<class T>
concept SpaceFieldObjectConcept = std::is_base_of_v<SpaceFieldObject, T> && !std::is_abstract_v<T>;
template<class T, class... Args>
concept ShipConcept
	= std::is_base_of_v<AbstractShip, T> && SpaceFieldObjectConcept<T> && requires(Args const &...args) { T(args...); };
template<class T>
concept BulletConcept = std::is_base_of_v<AbstractBullet, T> && SpaceFieldObjectConcept<T>;
template<class T>
concept AsteroidConcept = std::is_base_of_v<AbstractAsteroid, T> && SpaceFieldObjectConcept<T>;

class SpaceField : public sf::Drawable, public rn::LogicalObject
{
public:
	template<class T>
	using container_type = std::vector<T>;
	using value_type	 = std::unique_ptr<SpaceFieldObject>;
	using const_reference = const value_type &;
	using reference = value_type &;
	using objects_type	 = container_type<value_type>;
	using iterator		 = objects_type::iterator;
	using const_iterator = objects_type::const_iterator;
	using callback_type = std::function<void(SpaceFieldObject *)>;
	SpaceField(const Camera2d *camera = nullptr);
	SpaceField(const SpaceField &field) = delete;
	~SpaceField() override				= default;

	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	void setCamera(const Camera2d *camera2d);
	const Camera2d *getCamera() const;
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;

	const_reference at(size_t index) const;
	const_reference operator[](size_t index) const;

	size_t size() const;
	void clear();

	template<class T, class... Args>
		requires(ShipConcept<T, Args...>)
	AbstractShip *summonShip(const Args &...args) noexcept;
	template<BulletConcept BulletT>
	AbstractBullet *summonBullet(const std::function<void(BulletT &)> &init, const Gun *gun) noexcept;
	template<AsteroidConcept AsteroidT>
	AbstractAsteroid *summonAsteroid(const rn::Vec2f &summon_position = {}, const rn::Vec2f &velocity = {});
	void push_back(AbstractShip *ship);
	void push_back(AbstractBullet *bullet, const Gun *const &gun = nullptr);
	void push_back(AbstractAsteroid *asteroid, const rn::Vec2f &summon_position = {}, const rn::Vec2f &velocity = {});
	void destroy(const SpaceFieldObject *object);
	virtual void onObjectSummon(GameObject *object) const;
	virtual void onObjectDestroy(GameObject *object) const;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	SpaceField &operator=(const SpaceField &other) = delete;

private:
	void clearGarbage();
	const Camera2d *m_camera{nullptr};

	objects_type m_objects{};
};


template<class T, class... Args>
	requires(ShipConcept<T, Args...>)
AbstractShip *SpaceField::summonShip(const Args &...args) noexcept
{
	T *ship = new T(args...);
	ship->start();
	this->onObjectSummon(ship);
	m_objects.emplace_back(ship);
	return ship;
}

template<BulletConcept BulletT>
AbstractBullet *SpaceField::summonBullet(const std::function<void(BulletT &)> &init, const Gun *gun) noexcept
{
	BulletT *bullet = new BulletT;
	init(*bullet);
	bullet->author = gun;
	bullet->start();
	bullet->onSummon();
	this->onObjectSummon(bullet);
	m_objects.emplace_back(bullet);
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
	this->onObjectSummon(asteroid);
	m_objects.emplace_back(asteroid);
	return asteroid;
}
