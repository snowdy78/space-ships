#pragma once

#include <optional>
#include <variant>
#include "TransferableAction.hpp"
#include "TransferableObject.hpp"
#include "decl.hpp"

class BasicRouterResponse
{
	std::optional<rn::Json> m_response_data = std::nullopt;
	sf::Socket::Status m_status								  = sf::Socket::Done;
	BasicRouterResponse(const rn::Json &data_json = {});
	BasicRouterResponse(const sf::Socket::Status &status);
	template<class RespT>
	friend class BasicRouter;

public:
	static constexpr const char *id_key				= "id";
	static constexpr const char *data_key			= "data";
	static constexpr const char *author_id_key		= "author_id";
	static constexpr const char *contributor_id_key = "contributor_id";
	static constexpr const char *type_key			= "type";
	static constexpr const char *object_key = "object";
	static constexpr const char *action_key = "action";
	std::optional<size_t> id() const;
	std::optional<rn::Json> data() const;
	bool success() const;
	sf::Socket::Status status() const;
	bool is_unknown() const;
	bool is_action() const;
	bool is_object() const;
	std::unique_ptr<TransferableObject> object() const;
	std::unique_ptr<TransferableAction> action() const;
	const rn::Json &json() const;
};

template<class RespT>
class BasicRouter
{
public:
	using Response	   = RespT;
	using StatusOrJson = std::variant<sf::Socket::Status, rn::Json>;

	BasicRouter()		   = default;
	virtual ~BasicRouter() = default;
	sf::Socket::Status send(const TransferableObject *data);
	sf::Socket::Status send(TransferableAction *action);
	Response receive();

protected:
	virtual sf::Socket::Status sendJson(const rn::Json &) = 0;
	virtual StatusOrJson receiveJson()					  = 0;
};

template<class RespT>
sf::Socket::Status BasicRouter<RespT>::send(const TransferableObject *data)
{
	auto send_data	  = data->toJson();
	send_data[BasicRouterResponse::type_key] = BasicRouterResponse::object_key;
	return sendJson(send_data);
}

template<class RespT>
sf::Socket::Status BasicRouter<RespT>::send(TransferableAction *action)
{
	using namespace std::string_literals;
	if (!action)
		return sf::Socket::Status::Error;
	Transferable::TransferJson send_data = action->toJson();
	send_data[BasicRouterResponse::type_key]  = BasicRouterResponse::action_key;
	if (action->author)
		send_data[BasicRouterResponse::author_id_key] = *action->author;
	else
		send_data[BasicRouterResponse::author_id_key] = nullptr;
	if (action->contributor)
		send_data[BasicRouterResponse::author_id_key] = *action->contributor;
	else
		send_data[BasicRouterResponse::contributor_id_key] = nullptr;
	return sendJson(send_data);
}

template<class RespT>
typename BasicRouter<RespT>::Response BasicRouter<RespT>::receive()
{
	auto json_or_status = receiveJson();
	if (std::holds_alternative<sf::Socket::Status>(json_or_status))
		return std::get<sf::Socket::Status>(json_or_status);
	auto &json = std::get<rn::Json>(json_or_status);
	if (!json.contains(BasicRouterResponse::type_key) || !json.at(BasicRouterResponse::type_key).is_string())
		throw incorrect_json_format("invalid json format");
	auto type = json.at(BasicRouterResponse::type_key);
	if (type != BasicRouterResponse::action_key && type != BasicRouterResponse::object_key)
		throw incorrect_json_format("invalid send data operation");
	return Response(json);
}
