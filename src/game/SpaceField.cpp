#include "game/SpaceField.hpp"
#include <algorithm>

SpaceField::SpaceField(const Camera2d *camera)
	: m_camera(camera),
	  m_mother(camera)
{
}

void SpaceField::setCamera(const Camera2d *camera2d)
{
	m_camera = camera2d;
	m_mother.setCamera(camera2d);
}

const Camera2d *SpaceField::getCamera() const
{
	return m_camera;
}

const BulletMother &SpaceField::getBulletMother() const
{
	return m_mother;
}

void SpaceField::destroyShip(const AbstractShip *ship)
{
	auto it = std::ranges::find_if(m_ships, [ship](const ships_container::value_type &value) {
		return ship == value.get();
	});
	if (it != m_ships.end())
	{
		it->get()->need_destroy = true;
	}
}
AbstractShip *SpaceField::get(size_t index)
{
	return m_ships.at(index).get();
}

const AbstractShip *SpaceField::get(size_t index) const
{
	return m_ships.at(index).get();
}

AbstractShip *SpaceField::operator[](size_t index)
{
	return m_ships[index].get();
}

const AbstractShip *SpaceField::operator[](size_t index) const
{
	return m_ships[index].get();
}

const SpaceField::ships_container &SpaceField::getShips() const
{
	return m_ships;
}
void SpaceField::start()
{
	for (auto &ship: m_ships)
		ship->start();
	for (auto &asteroid: m_asteroids)
		asteroid->start();
	m_mother.start();
}
void SpaceField::update()
{
	garbageFree();
	for (auto &ship: m_ships)
		ship->update();
	for (auto &asteroid: m_asteroids)
		asteroid->update();
	m_mother.update();
}
void SpaceField::onEvent(sf::Event &event)
{
	for (auto &ship: m_ships)
		ship->onEvent(event);
	for (auto &asteroid: m_asteroids)
		asteroid->onEvent(event);
	m_mother.onEvent(event);
}
size_t SpaceField::size() const
{
	return m_ships.size();
}

void SpaceField::clear()
{
	m_ships.clear();
	m_asteroids.clear();
	m_mother.clear();
}

void SpaceField::push_back(AbstractShip *ship)
{
	if (!ship)
		throw std::runtime_error("Error: Cannot summon null on field");
	ship->start();
	m_ships.emplace_back(ship);
	onObjectAppend(ship);
}

void SpaceField::push_back(Bullet *bullet, const Gun * const &gun)
{
	if (!bullet)
		throw std::runtime_error("Error: Cannot summon null on field");
	if (gun)
		bullet->author = gun;
	m_mother.summon(bullet);
	bullet->start();
	bullet->onSummon();
	onObjectAppend(bullet);
}

void SpaceField::push_back(AbstractAsteroid *asteroid, const rn::Vec2f &summon_position, const rn::Vec2f &velocity)
{
	if (!asteroid)
		throw std::runtime_error("Error: Cannot summon null on field");
	asteroid->setPosition(summon_position);
	asteroid->setVelocity(static_cast<float>(rn::math::length(velocity)));
	asteroid->setDirection(rn::math::norm(velocity));
	asteroid->start();
	m_asteroids.emplace_back(asteroid);
	onObjectAppend(asteroid);
}

void SpaceField::destroyAsteroid(const AbstractAsteroid *asteroid)
{
	auto it = std::ranges::find_if(m_asteroids, [asteroid](const asteroid_ptr_t &obj) {
		return obj.get() == asteroid;
	});
	if (it != m_asteroids.end())
	{
		it->get()->need_destroy = true;
	}
}

void SpaceField::destroyBullet(Bullet *const &bullet)
{
	auto it = std::ranges::find_if(m_mother, [bullet](const BulletMother::value_type &child) {
		return bullet == child.get();
	});
	if (it != m_mother.end())
	{
		it->bullet->need_destroy = true;
	}
}


void SpaceField::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto &iterator: m_mother)
		if (auto bullet = iterator.get())
			target.draw(*bullet, states);
	for (auto &ship: m_ships)
		target.draw(*ship, states);
	for (auto &asteroid: m_asteroids)
		target.draw(*asteroid, states);
}

void SpaceField::garbageFree()
{
	std::erase_if(m_ships, [](const ships_container::value_type &ship) {
		return ship->need_destroy;
	});
	std::erase_if(m_asteroids, [](const asteroids_container::value_type &asteroid) {
		return asteroid->need_destroy;
	});
	std::erase_if(m_mother, [](const BulletMother::value_type &child_bullet) {
		return child_bullet.bullet->need_destroy;
	});
}
