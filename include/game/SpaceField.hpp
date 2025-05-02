#pragma once

#include "AbstractShip.hpp"
#include "BulletMother.hpp"
#include "decl.hpp"
#include "game/BulletMother.hpp"


template<class T>
concept ShipT = std::is_base_of<AbstractShip, T>::value && !std::is_same<T, AbstractShip>::value;

class SpaceField : public sf::Drawable, public rn::LogicalObject
{
public:
	template<class T>
	using container = std::vector<T>;
	using ship_t = AbstractShip *;
	using ships_container = container<ship_t>;
private:
	ships_container ships{};

	Camera2d *camera;
	BulletMother mother{camera};

public:
	using iterator		 = std::vector<AbstractShip *>::iterator;
	using const_iterator = std::vector<AbstractShip *>::const_iterator;

	SpaceField(Camera2d *camera = nullptr);
	SpaceField(const SpaceField &field);
	SpaceField(SpaceField &&) noexcept = default;
	~SpaceField();

	void setCamera(Camera2d *camera2d);
	const Camera2d *getCamera() const;
	const BulletMother &getBulletMother() const;
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;

	AbstractShip *get(size_t index);
	AbstractShip *operator[](size_t index);
	const ships_container &getShips() const;

	size_t size();
	void clear();

	template<ShipT T, class... Args>
	void appendShip(const Args &...args);
	void summonShip(AbstractShip *const &object);
	void summonBullet(Bullet *const &bullet, const rn::Vec2f &direction);
	void destroyBullet(const Bullet *const &bullet);

	void remove(const AbstractShip *ship);

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	SpaceField &operator=(const SpaceField &other);
	SpaceField &operator=(SpaceField &&other) noexcept;
};


template<ShipT T, class... Args>
void SpaceField::appendShip(const Args &...args)
{
	ships.push_back(new T(args...));
}
