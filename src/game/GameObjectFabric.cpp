#include "game/GameObjectFabric.hpp"
#include <stdexcept>
#include "coop/TransferableObject.hpp"
#include "game/GameObject.hpp"

size_t GameObjectFabricTranslator::id = identify<GameObjectFabricTranslator>();

void GameObjectFabric::emplace(size_t id, GameObject *object)
{
	objects.emplace(id, object);
	id_encounter = std::max(id_encounter, id);
}

GameObjectFabric::GameObjectFabric() = default;

GameObjectFabric &GameObjectFabric::instance()
{
	static GameObjectFabric instance;
	return instance;
}

GameObjectFabric::Iterator GameObjectFabric::begin()
{
	return objects.begin();
}

GameObjectFabric::ConstIterator GameObjectFabric::cbegin() const
{
	return objects.cbegin();
}

GameObjectFabric::ConstIterator GameObjectFabric::begin() const
{
	return objects.begin();
}

GameObjectFabric::Iterator GameObjectFabric::end()
{
	return objects.end();
}

GameObjectFabric::ConstIterator GameObjectFabric::cend() const
{
	return objects.cend();
}

GameObjectFabric::ConstIterator GameObjectFabric::end() const
{
	return objects.end();
}

std::vector<std::unique_ptr<TransferableObject>> GameObjectFabric::assign(const GameObjectFabricTranslator &translator)
{
	clear();
	return std::move(update(translator));
}

std::vector<std::unique_ptr<TransferableObject>> GameObjectFabric::update(const GameObjectFabricTranslator &translator)
{
	const auto &data = translator.data_to_update;
	std::vector<std::unique_ptr<TransferableObject>> result;
	for (auto &it : data)
	{
		if (objects.contains(it.first))
			continue;
		auto object = TransferableObjectFabric::instance().get(it.second)();
		// adding a new object
		if (auto game_object = dynamic_cast<GameObject *>(object.get()))
		{
			emplace(it.first, game_object);
			result.emplace_back(std::move(object));
		}
	}
	return std::move(result);
}

void GameObjectFabric::remove(const GameObjectFabricTranslator &translator, std::function<void(GameObject *)> on_remove)
{
	const auto &data = translator.data_to_update;
	for (auto &it : data)
	{
		if (!objects.contains(it.first))
			continue;
		auto object = get(it.first);
		on_remove(object);
		erase(it.first);
	}
}

GameObject *const &GameObjectFabric::get(size_t id)
{
	return objects.at(id);
}


size_t GameObjectFabric::push(GameObject *object)
{
	objects.emplace(id_encounter, object);
	return id_encounter++;
}

size_t GameObjectFabric::find(GameObject *object)
{
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		if (it->second == object)
			return it->first;
	}
	throw std::out_of_range("Object not found");
}

void GameObjectFabric::erase(size_t id)
{
	objects.erase(id);
}

void GameObjectFabric::clear()
{
	id_encounter = 0;
	objects.clear();
}

GameObjectFabricTranslator::GameObjectFabricTranslator() {}

void GameObjectFabricTranslator::receiveJson(const rn::Json &json)
{
	if (json.contains("type") && json["type"].is_number())
	{
		translate_type = static_cast<TranslateType>(json["type"].get<size_t>());
	}
	for (auto &it: json["ids"])
	{
		if (!it.is_array() || it.size() != 2)
			throw std::out_of_range("invalid json format");
		data_to_update.emplace(it[0], it[1]);
	}
}

GameObjectFabricTranslator::TranslateType GameObjectFabricTranslator::getTranslateType() const
{
	return translate_type;
}

void GameObjectFabricTranslator::setTranslateType(TranslateType type)
{
	translate_type = type;
}

void GameObjectFabricTranslator::assignUpdateData(
	GameObjectFabric::ConstIterator begin, const GameObjectFabric::ConstIterator &end
)
{
	clearUpdateData();
	for (auto it = begin; it != end; it++)
	{
		insert(it);
	}
}

GameObjectFabricTranslator::TransferJson GameObjectFabricTranslator::toJson() const
{
	rn::Json arr = rn::Json::array();
	for (auto &it: data_to_update)
	{
		arr.push_back(rn::Json::array({ it.first, it.second }));
	}
	return {
		id,
		{ { "type", static_cast<size_t>(translate_type) }, { "ids", arr } }
	};
}

bool GameObjectFabricTranslator::insert(GameObjectFabric::ConstIterator iterator)
{
	if (auto obj = dynamic_cast<TransferableObject *>(iterator->second))
	{
		data_to_update.emplace(iterator->first, obj->toJson().id());
		return true;
	}
	return false;
}

size_t GameObjectFabricTranslator::erase(size_t id)
{
	auto it = data_to_update.find(id);
	if (it != data_to_update.end())
	{
		data_to_update.erase(it);
		return id;
	}
	return npos;
}

size_t GameObjectFabricTranslator::updateCount() const
{
	return data_to_update.size();
}

void GameObjectFabricTranslator::clearUpdateData()
{
	data_to_update.clear();
}
size_t GameObjectFabric::size() const
{
	return objects.size();
}
