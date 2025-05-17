#pragma once

#include "AbstractShip.hpp"
#include "BulletMother.hpp"
#include "decl.hpp"
#include "game/BulletMother.hpp"


template<class T>
concept ShipConcept = std::is_base_of_v<AbstractShip, T> && !std::is_abstract_v<T>;
template<class T>
concept BulletConcept = std::is_base_of_v<Bullet, T> && !std::is_abstract_v<T>;

class SpaceField : public sf::Drawable, public rn::LogicalObject
{
public:
	template<class T>
	using container = std::vector<T>;
	using ship_ptr_t = AbstractShip *;
	using bullet_ptr_t = Bullet *;
	using ships_container = container<ship_ptr_t>;
private:
	ships_container ships{};

	Camera2d *camera;
	BulletMother mother{camera};

public:
	using iterator		 = std::vector<AbstractShip *>::iterator;
	using const_iterator = std::vector<AbstractShip *>::const_iterator;

	SpaceField(Camera2d *camera = nullptr);
	SpaceField(const SpaceField &field) = delete;
	SpaceField(SpaceField &&) noexcept = default;
	~SpaceField() override;

	void setCamera(Camera2d *camera2d);
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

	template<ShipConcept T, class... Args>
	void appendShip(const Args &...args) noexcept;
	template<BulletConcept BulletT>
	void summonBullet(const std::function<void(BulletT &)> &init, const Gun *gun) noexcept;
	void destroyBullet(const Bullet *const &bullet);
	virtual void onObjectAppend(GameObject *object) const
	{
	}
	void remove(const AbstractShip *ship);

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	SpaceField &operator=(const SpaceField &other) = delete;
	SpaceField &operator=(SpaceField &&other) noexcept;
};


template<ShipConcept T, class... Args>
void SpaceField::appendShip(const Args &...args) noexcept
{
	auto ship = new T(args...);
	ships.push_back(ship);
	this->onObjectAppend(ship);
}

template<BulletConcept BulletT>
void SpaceField::summonBullet(const std::function<void(BulletT &)> &init, const Gun *gun) noexcept
{
	Bullet *bullet = new BulletT;
	init(*bullet);
	bullet->author = gun;
	mother.summon(bullet);
	bullet->start();
	bullet->onSummon();
	this->onObjectAppend(bullet);
}
