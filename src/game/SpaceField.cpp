#include "game/SpaceField.hpp"

SpaceField::SpaceField(Camera2d *camera)
	: camera(camera),
	  mother(camera)
{
}

SpaceField::SpaceField(const SpaceField &field)
	: camera(field.camera)
{
	ships.resize(field.ships.size());
	for (size_t i = 0; i < field.ships.size(); ++i)
		ships[i] = field.ships[i]->copy();
}
SpaceField::~SpaceField()
{
	for (auto &ship: ships)
		delete ship;
}

void SpaceField::setCamera(Camera2d *camera2d)
{
	camera = camera2d;
	mother.setCamera(camera2d);
}

const Camera2d *SpaceField::getCamera() const
{
	return camera;
}

const BulletMother &SpaceField::getBulletMother() const
{
	return mother;
}

void SpaceField::remove(const AbstractShip *ship)
{
	auto it = std::find(ships.begin(), ships.end(), ship);
	if (it != ships.end())
	{
		delete *it;
		ships.erase(it);
	}
}
AbstractShip *SpaceField::get(size_t index)
{
	return ships.at(index);
}
AbstractShip *SpaceField::operator[](size_t index)
{
	return ships[index];
}
const SpaceField::ships_container &SpaceField::getShips() const
{
	return ships;
}
void SpaceField::start()
{
	for (auto &ship: ships)
		ship->start();
	mother.start();
}
void SpaceField::update()
{
	for (auto &ship: ships)
		ship->update();
	mother.update();
}
void SpaceField::onEvent(sf::Event &event)
{
	for (auto &ship: ships)
		ship->onEvent(event);
	mother.onEvent(event);
}
size_t SpaceField::size()
{
	return ships.size();
}

void SpaceField::clear()
{
	for (auto &ship: ships)
		delete ship;
	ships.clear();
}

void SpaceField::summonBullet(Bullet *const &bullet, const rn::Vec2f &direction)
{
	if (bullet)
	{
		mother.summon(bullet, direction);
		bullet->onSummon();
	}
}

void SpaceField::destroyBullet(const Bullet *const &bullet)
{
	mother.destroy(bullet);
}
void SpaceField::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto &iterator: mother)
		if (auto bullet = iterator.get())
			target.draw(*bullet, states);
	for (auto &ship: ships)
		target.draw(*ship, states);
}


SpaceField &SpaceField::operator=(const SpaceField &other)
{
	if (&other != this)
	{
		camera = other.camera;
		for (auto &ship: ships)
			delete ship;
		ships = other.ships;
	}
	return *this;
}
SpaceField &SpaceField::operator=(SpaceField &&other) noexcept
{
	if (&other != this)
	{
		camera = std::move(other.camera);
		mother = std::move(other.mother);
		for (auto &ship: ships)
			delete ship;
		ships = std::move(other.ships);
	}
	return *this;
}
