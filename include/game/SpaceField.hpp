#pragma once

#include "Camera2d.hpp"
#include "SpaceFieldObject.hpp"
#include "SpaceItem.hpp"
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

template<class T>
concept SpaceItemConcept = std::is_base_of_v<SpaceItem, T>;
template<class W>
concept WeaponConcept = std::is_base_of_v<AbstractWeapon, W> && std::is_final_v<W> && SpaceItemConcept<W>;

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
	using InitializerFunc	  = std::function<void(T &)>;
	using ItemType			  = SmartPtrType<SpaceItem>;
	using StateType			  = SmartPtrType<SpaceFieldObject>;
	using StatePtrType		  = StatePtr<SpaceFieldObject>;
	using ValueType			  = StateType;
	using ObjectContainerType = ContainerType<StateType>;
	using ItemContainerType	  = ContainerType<ItemType>;
	using Iterator			  = ObjectContainerType::iterator;
	using ConstIterator		  = ObjectContainerType::const_iterator;
	using CallbackType		  = std::function<void(SpaceFieldObject *)>;
	using ItemIterator		  = ItemContainerType::iterator;
	using ItemConstIterator = ItemContainerType::const_iterator;
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
	const ItemContainerType &items() const;
	template<SpaceItemConcept T, class ...Args>
	StatePtr<SpaceItem> emplaceItem(Args &&...args);
	StatePtr<SpaceItem> pushItem(SpaceItem *item);
	State<SpaceItem> take(const ItemConstIterator &iterator);
	ItemConstIterator find(const StatePtr<SpaceItem> &ptr) const;
	template<class T, class... Args>
		requires(ShipConcept<T, Args...>)
	StatePtr<AbstractShip> summonShip(const InitializerFunc<T> & = [](T &) {}) noexcept;
	template<BulletConcept BulletT>
	StatePtr<AbstractBullet> summonBullet(const InitializerFunc<BulletT> & = [](BulletT &) {}) noexcept;
	template<AsteroidConcept AsteroidT>
	StatePtr<AbstractAsteroid> summonAsteroid(const InitializerFunc<AsteroidT> & = [](AsteroidT &) {}) noexcept;
	StatePtrType push_back(SpaceFieldObject *raw_object);
	void destroy(const SpaceFieldObject *object);
	virtual void onObjectSummon(const StatePtrType &state_ptr) const;
	virtual void onObjectDestroy(const StatePtrType &state_ptr) const;
	template<class T>
	ConstIterator find(const T *object) const;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	SpaceField &operator=(const SpaceField &other) = delete;

private:
	void clearGarbage();
	template<class T>
	StatePtr<T> itemPush(const SmartPtrType<T> &ptr);
	template<SpaceFieldObjectConcept T>
	StatePtr<T> casted_push(T *ptr) noexcept;
	const Camera2d *m_camera{ nullptr };
	ObjectContainerType m_objects{};
	ItemContainerType m_items;
};

template<SpaceItemConcept T, class... Args>
SpaceField::StatePtr<SpaceItem> SpaceField::emplaceItem(Args &&...args)
{
	return this->itemPush<T>(std::make_shared<T>(std::forward<Args>(args)...));
}

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
SpaceField::StatePtr<AbstractAsteroid> SpaceField::summonAsteroid(const InitializerFunc<AsteroidT> &init) noexcept
{
	AsteroidT *asteroid = new AsteroidT;
	init(*asteroid);
	return this->casted_push<AbstractAsteroid>(asteroid);
}

template<class T>
SpaceField::ConstIterator SpaceField::find(const T *object) const
{
	return std::find_if(begin(), end(), [object](const ValueType &state) {
		return dynamic_cast<const T *>(state.get()) == object;
	});
}

template<class T>
SpaceField::StatePtr<T> SpaceField::itemPush(const SmartPtrType<T> &ptr)
{
	m_items.push_back(ptr);
	ptr->m_self = ptr;
	ptr->start();
	ptr->onSummon();
	return ptr;
}

template<SpaceFieldObjectConcept T>
SpaceField::StatePtr<T> SpaceField::casted_push(T *ptr) noexcept
{
	State<T> object{ ptr };

	if (auto obj = std::dynamic_pointer_cast<SpaceFieldObject>(object))
	{
		m_objects.push_back(obj);
		object->m_self = obj;
		object->start();
		object->onSummon();
		this->onObjectSummon(object);
		return object;
	}
	return {};
}
