#include "coop/Router.hpp"
#include "coop/TransferableAction.hpp"
#include "game/GameObjectFabric.hpp"

BasicRouterResponse::BasicRouterResponse(const rn::Json &data_json)
{
	if (!(data_json.contains("id") && data_json.contains("data")))
	{
		std::cerr << "BasicRouterResponse.constructor: 'data_json' does not contains id, data keys. 'data_json' look: \n";
		std::cerr << data_json.dump(2, ' ', '\n') << "\n";
		m_status = sf::Socket::Error;
		return;
	}
	transfer_json = std::make_unique<Transferable::TransferJson>(data_json.at("id"), data_json.at("data"));

}

BasicRouterResponse::BasicRouterResponse(const sf::Socket::Status &status)
	: m_status(status)
{}

bool BasicRouterResponse::success() 
{
	return bool(transfer_json);
}

sf::Socket::Status BasicRouterResponse::status() 
{
    return m_status;
}

bool BasicRouterResponse::is_unknown() const
{
	return !is_action() && !is_object();
}

bool BasicRouterResponse::is_action() const
{
	return (*transfer_json)["type"] == TransferType::action;
}

bool BasicRouterResponse::is_object() const
{
	return (*transfer_json)["type"] == TransferType::object;
}

std::unique_ptr<TransferableAction> BasicRouterResponse::action() const
{
	if (!is_action())
		return nullptr;
	try
	{
		auto &author = GameObjectFabric::instance().get((*transfer_json)["author_id"]);
		auto &target = GameObjectFabric::instance().get((*transfer_json)["target_id"]);
		return std::move(
			TransferableActionFabric::instance().get(transfer_json->id())(author, target, transfer_json->data())
		);
	}
	catch (rn::Json::out_of_range &err)
	{}
	return nullptr;
}

const Transferable::TransferJson &BasicRouterResponse::json() const
{
	return (*transfer_json);
}

std::unique_ptr<TransferableObject> BasicRouterResponse::object() const
{
	if (!is_object())
		return nullptr;
	try
	{
		auto transferable = TransferableObjectFabric::instance().get(transfer_json->id())();
		transferable->receiveJson(transfer_json->data());
		return std::move(transferable);
	}
	catch (std::out_of_range &err)
	{}
	return nullptr;
}
