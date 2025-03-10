#include "game/GameObjectFabric.hpp"

GameObjectFabric::GameObjectFabric() = default;

GameObjectFabric &GameObjectFabric::instance()
{
	static GameObjectFabric instance;
	return instance;
}

void GameObjectFabric::assignReceived(const rn::Json &fabric_json)
{
	clear();
	if (!fabric_json.contains("data") || !fabric_json["data"].is_object())
		throw std::out_of_range("invalid json format");
	auto fabric_data_json = fabric_json["data"];
	if (!fabric_data_json.contains("id_encounter") || !fabric_data_json["id_encounter"].is_number()
		|| !fabric_data_json.contains("ids") || !fabric_data_json["ids"].is_array())
		throw std::out_of_range("invalid json format");
	for (auto &it: fabric_data_json["ids"])
	{
		if (!it.is_array() || it.size() != 2)
		{
			clear();
			throw std::out_of_range("invalid json format");
		}
		objects.emplace(it[0], TransferableFabric::instance().get(it[1]));
	}
	id_encounter = fabric_data_json["id_encounter"];
}

GameObject *GameObjectFabric::get(size_t id)
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

rn::Json GameObjectFabricTranslator::toJson() const
{
	rn::Json json;
	json["id"]			 = id;
	json["id_encounter"] = GameObjectFabric::instance().id_encounter;
	json["ids"]			 = rn::Json::array();
	for (auto &it: GameObjectFabric::instance().objects)
	{
		if (auto transferable = dynamic_cast<Transferable *>(it.second))
		{
			json["ids"].push_back(rn::Json::array({ it.first, transferable->toJson()["id"] }));
		}
	}
	return json;
}
