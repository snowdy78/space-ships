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
	using State = SmartPtrType<T>;
	template<class T>
	using InitializerFunc = std::function<void(T &)>;
	using StateType		  = SmartPtrType<SpaceFieldObject>;
	using StatePtrType	  = StatePtr<SpaceFieldObject>;
	using ObjectsType	  = ContainerType<StateType>;
	using Iterator		  = ObjectsType::iterator;
	using ConstIterator	  = ObjectsType::const_iterator;
	using CallbackType	  = std::function<void(SpaceFieldObject *)>;
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
	StatePtr<AbstractShip> summonShip(const InitializerFunc<T> & = [](T &) {}) noexcept;
	template<BulletConcept BulletT>
	StatePtr<AbstractBullet> summonBullet(const InitializerFunc<BulletT> & = [](BulletT &) {}) noexcept;
	template<AsteroidConcept AsteroidT>
	StatePtr<AbstractAsteroid> summonAsteroid(const InitializerFunc<AsteroidT> & = [](AsteroidT &) {});
	StatePtrType push_back(SpaceFieldObject *raw_object);
	void destroy(const SpaceFieldObject *object);
	virtual void onObjectSummon(const StatePtrType &state_ptr) const;
	virtual void onObjectDestroy(const StatePtrType &state_ptr) const;

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
SpaceField::StatePtr<AbstractShip> SpaceField::summonShip(const InitializerFunc<T> &init) noexcept
{
	T *ship = new T;
	init(*ship);
	return this->casted_push<AbstractShip>(ship);
}

template<BulletConcept BulletT>
SpaceField::StatePtr<AbstractBullet> SpaceField::summonBullet(const InitializerFunc<BulletT> &init) noexcept
{
	BulletT *bullet = new BulletT;
	init(*bullet);
	return this->casted_push<AbstractBullet>(bullet);
}

template<AsteroidConcept AsteroidT>
SpaceField::StatePtr<AbstractAsteroid> SpaceField::summonAsteroid(const InitializerFunc<AsteroidT> &init)
{
	AsteroidT *asteroid = new AsteroidT;
	init(*asteroid);
	return this->casted_push<AbstractAsteroid>(asteroid);
}

template<SpaceFieldObjectConcept T>
SpaceField::StatePtr<T> SpaceField::casted_push(T *ptr)
{
	State<T> object{ ptr };
	if (auto obj = std::dynamic_pointer_cast<SpaceFieldObject>(object))
	{
		m_objects.push_back(obj);
		object->start();
		object->onSummon();
		this->onObjectSummon(object);
		return object;
	}
	return {};
}
