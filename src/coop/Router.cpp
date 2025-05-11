#include "coop/Router.hpp"
#include "coop/TransferableAction.hpp"
#include "game/GameObjectFabric.hpp"

BasicRouterResponse::BasicRouterResponse(const rn::Json &data_json)
{
	if (!(data_json.contains("id") && data_json.contains("data")))
	{
		std::cerr
			<< "BasicRouterResponse.constructor: 'data_json' does not contains id, data keys. 'data_json' look: \n";
		std::cerr << data_json.dump(2, ' ', '\n') << "\n";
		m_status = sf::Socket::Error;
		return;
	}
	m_response_data.emplace(data_json);
}

BasicRouterResponse::BasicRouterResponse(const sf::Socket::Status &status)
	: m_response_data(std::nullopt), m_status(status)
{
}

std::optional<size_t> BasicRouterResponse::id() const
{
	if (!success())
		return std::nullopt;
	return m_response_data->at(id_key);
}

std::optional<rn::Json> BasicRouterResponse::data() const
{
	if (!success())
		return std::nullopt;
	return m_response_data->at(data_key);
}

bool BasicRouterResponse::success() const
{
	return m_response_data.has_value();
}

sf::Socket::Status BasicRouterResponse::status() const
{
	return m_status;
}

bool BasicRouterResponse::is_unknown() const
{
	return !is_action() && !is_object();
}

bool BasicRouterResponse::is_action() const
{
	if (!success())
		return false;
	return m_response_data->at(type_key) == action_key;
}

bool BasicRouterResponse::is_object() const
{
	if (!success())
		return false;
	return m_response_data->at(type_key) == object_key;
}

std::unique_ptr<TransferableAction> BasicRouterResponse::action() const
{
	if (!is_action())
		throw std::bad_cast();
	try
	{
		const auto author_id		= m_response_data->at(author_id_key);
		const auto contributor_id = m_response_data->at(contributor_id_key);
		GameObject *author, *contributor;
		if (!author_id.is_null())
			author = GameObjectFabric::instance().get(author_id);
		else
			author = nullptr;
		if (!contributor_id.is_null())
			contributor = GameObjectFabric::instance().get(contributor_id);
		else
			contributor = nullptr;
		return TransferableActionFabric::instance().get(*id())(author, contributor, *data());
	}
	catch (rn::Json::out_of_range &err)
	{
		std::cerr << err.what()
				  << " (probably author or contributor of action not found in game objects fabric) \n";
		throw err;
	}
}

const rn::Json &BasicRouterResponse::json() const
{
	if (!m_response_data.has_value())
		throw std::out_of_range("no response data contains");
	return *m_response_data;
}

std::unique_ptr<TransferableObject> BasicRouterResponse::object() const
{
	if (!is_object())
		throw std::bad_cast();
	try
	{
		auto transferable = TransferableObjectFabric::instance().get(*id())();
		transferable->receiveJson(*data());
		return transferable;
	}
	catch (std::out_of_range &err)
	{
		std::cerr << err.what() << "\n";
		throw err;
	}
}
