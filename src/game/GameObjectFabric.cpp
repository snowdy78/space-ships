#include "game/GameObjectFabric.hpp"
#include "game/GameObject.hpp"

size_t GameObjectFabricTranslator::id = identify<GameObjectFabricTranslator>();

GameObjectFabric::GameObjectFabric() = default;

GameObjectFabric &GameObjectFabric::instance()
{
	static GameObjectFabric instance;
	return instance;
}

std::vector<std::unique_ptr<TransferableObject>> GameObjectFabric::assign(const GameObjectFabricTranslator &translator)
{
	clear();
	auto fabric_json = translator.toJson();
	if (!fabric_json.contains("data") || !fabric_json["data"].is_object())
		throw std::out_of_range("invalid json format");
	auto fabric_data_json = fabric_json["data"];
	if (!fabric_data_json.contains("id_encounter") || !fabric_data_json["id_encounter"].is_number()
		|| !fabric_data_json.contains("ids") || !fabric_data_json["ids"].is_array())
		throw std::out_of_range("invalid json format");
	std::vector<std::unique_ptr<TransferableObject>> dynamic_objects{};
	for (auto &it: fabric_data_json["ids"])
	{
		if (!it.is_array() || it.size() != 2)
		{
			clear();
			throw std::out_of_range("invalid json format");
		}
		auto object = TransferableObjectFabric::instance().get(it[1])();
		if (auto game_object = dynamic_cast<GameObject *>(object.get()))
		{
			objects.emplace(it[0], game_object);
		}
		dynamic_objects.emplace_back(std::move(object));
	}
	id_encounter = fabric_data_json["id_encounter"];
	return dynamic_objects;
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

size_t GameObjectFabric::getByValue(GameObject *object)
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
    
}

GameObjectFabricTranslator::TransferJson GameObjectFabricTranslator::toJson() const
{
	rn::Json arr = rn::Json::array();
	for (auto &it: GameObjectFabric::instance().objects)
	{
		if (auto transferable = dynamic_cast<TransferableObject *>(it.second))
		{
			arr.push_back(rn::Json::array({ it.first, transferable->toJson()["id"] }));
		}
	}
	return {
		id,
		{ { "id_encounter", GameObjectFabric::instance().id_encounter }, { "ids", arr } }
	};
}
