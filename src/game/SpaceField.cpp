#include "game/SpaceField.hpp"

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

const SpaceField::ItemContainerType & SpaceField::items() const
{
	return m_items;
}

SpaceField::State<SpaceItem> SpaceField::take(const ItemConstIterator &iterator)
{
	ItemContainerType::value_type ptr = *iterator;
	m_items.erase(iterator);
	return ptr;
}

SpaceField::ItemConstIterator SpaceField::find(const StatePtr<SpaceItem> &ptr) const
{
	if (ptr.expired())
		return items().end();
	return std::ranges::find_if(items(), [&ptr](const ItemContainerType::value_type &item) {
		return item == ptr.lock();
	});
}

SpaceField::StatePtrType SpaceField::push_back(SpaceFieldObject *raw_object)
{
	if (!raw_object)
		throw std::runtime_error("Error: Cannot summon null on field");
	return casted_push<SpaceFieldObject>(raw_object);
}
void SpaceField::destroy(const SpaceFieldObject *object)
{
	auto it = std::ranges::find_if(m_objects, [object](const StateType &value) {
		return object == value.get();
	});
	if (it != end())
	{
		it->get()->need_destroy = true;
		onObjectDestroy(*it);
	}
}

void SpaceField::onObjectSummon(const StatePtrType &state_ptr) const
{
}

void SpaceField::onObjectDestroy(const StatePtrType &state_ptr) const
{
}

void SpaceField::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto &item: m_items)
		target.draw(*item, states);
	for (auto &iterator: m_objects)
		target.draw(*iterator, states);
}

void SpaceField::clearGarbage()
{
	std::erase_if(m_objects, [](const ValueType &value) {
		return value->need_destroy;
	});
}
