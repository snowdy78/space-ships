#include "game/GameObjectTranslator.hpp"
#include "coop/Router.hpp"

GameObjectTranslator::GameObjectTranslator() = default;

void GameObjectTranslator::receiveJson(const rn::Json &json)
{
	if (json.contains("type") && json["type"].is_number())
		translate_type = static_cast<TranslateType>(json["type"].get<size_t>());
	data_to_update.clear();
	for (auto &it: json["ids"].items())
	{
		size_t obj_id = std::stoi(it.key());
		auto data	  = it.value();
		TransferJson transfer_json{ data.at("id"), data.at("data") };

		data_to_update.emplace(obj_id, transfer_json);
#ifdef SPACE_SHIP_DEBUG
		std::cout << "data_to_update[\"" << obj_id << "\"] = " << data.dump() << "\n";
#endif
	}
}

rn::Json GameObjectTranslator::toJson() const
{
	rn::Json arr;
	for (auto &it: data_to_update)
	{
		auto key = std::to_string(it.first);
		arr[key] = static_cast<const rn::Json &>(it.second); // do not remove static cast, it's an exception escaping
	}
	return {
		{ "type", static_cast<size_t>(translate_type) },
		{ "ids",	 arr								 }
	};
}

GameObjectTranslator::TranslateType GameObjectTranslator::getTranslateType() const
{
	return translate_type;
}

void GameObjectTranslator::setTranslateType(TranslateType type)
{
	translate_type = type;
}

void GameObjectTranslator::assignUpdateData(
	GameObjectFactory::ConstIterator begin, const GameObjectFactory::ConstIterator &end
)
{
	clearUpdateData();
	for (auto it = begin; it != end; ++it)
		insert(it);
}

bool GameObjectTranslator::insert(GameObjectFactory::ConstIterator iterator)
{
	if (auto obj = dynamic_cast<TransferableObject *>(iterator->second))
	{
		auto obj_json = Router::prepareObject(obj->requestData());
		data_to_update.emplace(iterator->first, obj_json);
		return true;
	}
	return false;
}

size_t GameObjectTranslator::erase(size_t id)
{
	auto it = data_to_update.find(id);
	if (it != data_to_update.end())
	{
		data_to_update.erase(it);
		return id;
	}
	return npos;
}

size_t GameObjectTranslator::updateCount() const
{
	return data_to_update.size();
}

void GameObjectTranslator::clearUpdateData()
{
	data_to_update.clear();
}
