#pragma once

#include "AbstractAsteroid.hpp"
#include "AbstractBullet.hpp"
#include "AbstractShip.hpp"
#include "Camera2d.hpp"
#include "SpaceFieldObject.hpp"
#include "decl.hpp"

template<class T>
concept SpaceFieldObjectConcept = std::is_base_of_v<SpaceFieldObject, T>;
template<class T, class... Args>
concept ShipConcept = std::is_base_of_v<AbstractShip, T> && SpaceFieldObjectConcept<T> && !std::is_abstract_v<T>
					  && requires(Args const &...args) { T(args...); };
template<class T>
concept BulletConcept = std::is_base_of_v<AbstractBullet, T> && SpaceFieldObjectConcept<T> && !std::is_abstract_v<T>;
template<class T>
concept AsteroidConcept
	= std::is_base_of_v<AbstractAsteroid, T> && SpaceFieldObjectConcept<T> && !std::is_abstract_v<T>;

class SpaceField : public sf::Drawable, public rn::LogicalObject
{
public:
	template<class T>
	using ContainerType = std::vector<T>;
	template<class T>
	using SmartPtrType = std::shared_ptr<T>;
	template<class T>
	using StatePtr = std::weak_ptr<T>;
	template<class T>
	using State			= SmartPtrType<T>;
	using StateType		= SmartPtrType<SpaceFieldObject>;
	using StatePtrType	= StatePtr<SpaceFieldObject>;
	using ObjectsType	= ContainerType<StateType>;
	using Iterator		= ObjectsType::iterator;
	using ConstIterator = ObjectsType::const_iterator;
	using CallbackType	= std::function<void(SpaceFieldObject *)>;
	SpaceField(const Camera2d *camera = nullptr);
	SpaceField(const SpaceField &field) = delete;
	~SpaceField() override				= default;

	ConstIterator begin() const;
	ConstIterator end() const;
	ConstIterator cbegin() const;
	ConstIterator cend() const;

	void setCamera(const Camera2d *camera2d);
	const Camera2d *getCamera() const;
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;

	StatePtrType at(size_t index) const;
	StatePtrType operator[](size_t index) const;

	size_t size() const;
	void clear();

	template<class T, class... Args>
		requires(ShipConcept<T, Args...>)
	StatePtr<AbstractShip> summonShip(const Args &...args) noexcept;
	template<BulletConcept BulletT>
	StatePtr<AbstractBullet>
	summonBullet(const std::function<void(BulletT &)> &init, const AbstractWeapon *gun) noexcept;
	template<AsteroidConcept AsteroidT>
	StatePtr<AbstractAsteroid> summonAsteroid(const rn::Vec2f &summon_position = {}, const rn::Vec2f &velocity = {});
	StatePtrType push_back(AbstractShip *ship);
	StatePtrType push_back(AbstractBullet *bullet, const AbstractWeapon *const &gun = nullptr);
	StatePtrType
	push_back(AbstractAsteroid *asteroid, const rn::Vec2f &summon_position = {}, const rn::Vec2f &velocity = {});
	void destroy(const SpaceFieldObject *object);
	virtual void onObjectSummon(GameObject *object) const;
	virtual void onObjectDestroy(GameObject *object) const;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	SpaceField &operator=(const SpaceField &other) = delete;

private:
	void clearGarbage();
	template<SpaceFieldObjectConcept T>
	StatePtr<T> casted_push(T *ptr);

private:
	const Camera2d *m_camera{ nullptr };
	ObjectsType m_objects{};
};

template<class T, class... Args>
	requires(ShipConcept<T, Args...>)
SpaceField::StatePtr<AbstractShip> SpaceField::summonShip(const Args &...args) noexcept
{
	T *ship = new T(args...);
	ship->start();
	return this->casted_push<AbstractShip>(ship);
}

template<BulletConcept BulletT>
SpaceField::StatePtr<AbstractBullet>
SpaceField::summonBullet(const std::function<void(BulletT &)> &init, const AbstractWeapon *gun) noexcept
{
	BulletT *bullet = new BulletT;
	init(*bullet);
	bullet->author = gun;
	bullet->start();
	bullet->onSummon();

	return this->casted_push<AbstractBullet>(bullet);
}

template<AsteroidConcept AsteroidT>
SpaceField::StatePtr<AbstractAsteroid>
SpaceField::summonAsteroid(const rn::Vec2f &summon_position, const rn::Vec2f &velocity)
{
	AsteroidT *asteroid = new AsteroidT;
	asteroid->setPosition(summon_position);
	asteroid->setVelocity(static_cast<float>(rn::math::length(velocity)));
	asteroid->setDirection(rn::math::norm(velocity));
	asteroid->start();

	return this->casted_push<AbstractAsteroid>(asteroid);
}

template<SpaceFieldObjectConcept T>
SpaceField::StatePtr<T> SpaceField::casted_push(T *ptr)
{
	std::shared_ptr<T> shared{ ptr };
	if (auto space_field_obj = std::dynamic_pointer_cast<SpaceFieldObject>(shared))
		m_objects.push_back(space_field_obj);
	else
		throw std::bad_cast();
	this->onObjectSummon(ptr);
	return shared;
}
