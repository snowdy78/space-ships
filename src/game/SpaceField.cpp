#include "game/SpaceField.hpp"
#include "game/AbstractShip.hpp"
#include "game/AbstractBullet.hpp"
#include "game/AbstractAsteroid.hpp"

SpaceField::SpaceField(const Camera2d *camera)
	: m_camera(camera)
{
}

SpaceField::ConstIterator SpaceField::begin() const
{
	return m_objects.begin();
}

SpaceField::ConstIterator SpaceField::end() const
{
	return m_objects.end();
}

SpaceField::ConstIterator SpaceField::cbegin() const
{
	return m_objects.cbegin();
}

SpaceField::ConstIterator SpaceField::cend() const
{
	return m_objects.cend();
}

void SpaceField::setCamera(const Camera2d *camera2d)
{
	m_camera = camera2d;
}

const Camera2d *SpaceField::getCamera() const
{
	return m_camera;
}

SpaceField::StatePtrType SpaceField::at(size_t index) const
{
	return m_objects.at(index);
}

SpaceField::StatePtrType SpaceField::operator[](size_t index) const
{
	return m_objects[index];
}

void SpaceField::start()
{
	for (auto &object: m_objects)
		object->start();
}
void SpaceField::update()
{
	for (auto &object: m_objects)
		object->update();
	// clear garbage after update state
	clearGarbage();
}
void SpaceField::onEvent(sf::Event &event)
{
	for (auto &object: m_objects)
		object->onEvent(event);
}
size_t SpaceField::size() const
{
	return m_objects.size();
}

void SpaceField::clear()
{
	m_objects.clear();
}

void SpaceField::push_back(AbstractShip *ship)
{
	if (!ship)
		throw std::runtime_error("Error: Cannot summon null on field");
	ship->start();
	m_objects.emplace_back(ship);
	onObjectSummon(ship);
}

void SpaceField::push_back(AbstractBullet *bullet, const AbstractWeapon * const &gun)
{
	if (!bullet)
		throw std::runtime_error("Error: Cannot summon null on field");
	if (gun)
		bullet->author = gun;
	bullet->start();
	bullet->onSummon();
	m_objects.emplace_back(bullet);
	onObjectSummon(bullet);
}

void SpaceField::push_back(AbstractAsteroid *asteroid, const rn::Vec2f &summon_position, const rn::Vec2f &velocity)
{
	if (!asteroid)
		throw std::runtime_error("Error: Cannot summon null on field");
	asteroid->setPosition(summon_position);
	asteroid->setVelocity(static_cast<float>(rn::math::length(velocity)));
	asteroid->setDirection(rn::math::norm(velocity));
	asteroid->start();
	m_objects.emplace_back(asteroid);
	onObjectSummon(asteroid);
}

void SpaceField::destroy(const SpaceFieldObject *object)
{
	auto it = std::ranges::find_if(m_objects, [object](const StateType &value) {
		return object == value.get();
	});
	if (it != end())
	{
		it->get()->need_destroy = true;
		onObjectDestroy(it->get());
	}
}

void SpaceField::onObjectSummon(GameObject *object) const
{
}

void SpaceField::onObjectDestroy(GameObject *object) const
{
}

void SpaceField::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto &iterator: m_objects)
		target.draw(*iterator, states);
}

void SpaceField::clearGarbage()
{
	std::erase_if(m_objects, [](const StateType &value) {
		return value->need_destroy;
	});
}
