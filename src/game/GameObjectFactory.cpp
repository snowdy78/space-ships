#include "game/GameObjectFactory.hpp"
#include <stdexcept>

#include "coop/Router.hpp"
#include "coop/TransferDataConverter.hpp"
#include "coop/TransferableObject.hpp"
#include "game/GameObject.hpp"
#include "game/GameObjectTranslator.hpp"

void GameObjectFactory::emplace(size_t id, GameObject *object)
{
	objects.emplace(id, object);
	id_encounter = std::max(id_encounter, id);
}

GameObjectFactory::GameObjectFactory() = default;

GameObjectFactory &GameObjectFactory::instance()
{
	static GameObjectFactory instance;
	return instance;
}

std::unique_ptr<GameObject> GameObjectFactory::create(size_t id)
{
	if (!instance().m_factory.contains(id))
	{
		std::cerr << "GameObjectFactory does not register this identifier, registered identifiers: { ";
		for (auto &item : instance().m_factory)
		{
			std::cerr << item.first << ", ";
		}
		std::cerr << "}\n";
		throw std::out_of_range("std::out_of_range");
	}
	return std::unique_ptr<GameObject>{ instance().m_factory.at(id)() };
}

GameObjectFactory::Iterator GameObjectFactory::begin()
{
	return objects.begin();
}

GameObjectFactory::ConstIterator GameObjectFactory::cbegin() const
{
	return objects.cbegin();
}

GameObjectFactory::ConstIterator GameObjectFactory::begin() const
{
	return objects.begin();
}

GameObjectFactory::Iterator GameObjectFactory::end()
{
	return objects.end();
}

GameObjectFactory::ConstIterator GameObjectFactory::cend() const
{
	return objects.cend();
}

GameObjectFactory::ConstIterator GameObjectFactory::end() const
{
	return objects.end();
}

void GameObjectFactory::update(
	const GameObjectTranslator &translator,
	const emplace_callback_fn &emplace_outside
)
{
	clear();
	const auto &data = translator.data_to_update;
	for (auto &it: data)
	{
		if (objects.contains(it.first))
			continue;
		std::unique_ptr<GameObject> game_object{m_factory.at(it.second.id())()};
		if (auto object = dynamic_cast<TransferableObject *>(game_object.get()))
		{
			object->receiveJson(it.second.data());
			emplace(it.first, game_object.get());
			emplace_outside(std::unique_ptr<GameObject>(game_object.release()));
		}
	}
}

void GameObjectFactory::remove(const GameObjectTranslator &translator, const remove_callback_fn &callback)
{
	const auto &data = translator.data_to_update;
	for (auto &it: data)
	{
		if (!objects.contains(it.first))
			continue;
		auto object = get(it.first);
		callback(object);
		erase(it.first);
	}
}

GameObject *const &GameObjectFactory::get(size_t id)
{
	return objects.at(id);
}

const GameObject *const &GameObjectFactory::get(size_t id) const
{
	return objects.at(id);
}


size_t GameObjectFactory::push(GameObject *object)
{
	objects.emplace(id_encounter, object);
	return id_encounter++;
}

size_t GameObjectFactory::find(const GameObject *object) const
{
	for (auto &it: objects)
		if (it.second == object)
			return it.first;
	throw std::out_of_range("Object not found");
}

bool GameObjectFactory::contains(const GameObject *object) const
{
	auto it = std::ranges::find_if(objects, [object](std::pair<size_t, GameObject *> other) {
		return object == other.second;
	});
	return it != objects.end();
}

bool GameObjectFactory::can_create(size_t id) const
{
	return m_factory.contains(id);
}

void GameObjectFactory::erase(size_t id)
{
	objects.erase(id);
}

void GameObjectFactory::clear()
{
	id_encounter = 0;
	objects.clear();
}

size_t GameObjectFactory::size() const
{
	return objects.size();
}
