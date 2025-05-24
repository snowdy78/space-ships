#pragma once

#include <optional>
#include <variant>
#include "TransferableAction.hpp"
#include "TransferableObject.hpp"
#include "BasicRouterResponse.hpp"
#include "TransferDataConverter.hpp"
#include "decl.hpp"

enum class TransferType
{
	Tcp, Udp
};

template<class RespT>
class BasicRouter
{

public:
	using Response	   = RespT;
	using StatusOrJson = std::variant<sf::Socket::Status, rn::Json>;

	BasicRouter()		   = default;
	virtual ~BasicRouter() = default;
	static Transferable::TransferJson prepareObject(const Transferable::TransferJson &);
	sf::Socket::Status send(const TransferableObject *data);
	sf::Socket::Status send(const TransferableAction *action);
	Response receive();

protected:
	virtual sf::Socket::Status sendJson(const rn::Json &) = 0;
	virtual StatusOrJson receiveJson()					  = 0;
};

template<class RespT>
Transferable::TransferJson BasicRouter<RespT>::prepareObject(const Transferable::TransferJson &transfer_data)
{
	if (auto conversion = TransferDataConverter::instance().find(transfer_data.id()); conversion.has_value())
	{
		std::cout << "converting object...\n"; 
		return { conversion->identifier, conversion->convert(transfer_data.data()) };
	}
	return transfer_data;
}

template<class RespT>
sf::Socket::Status BasicRouter<RespT>::send(const TransferableObject *data)
{
	auto send_data	  = data->requestData();
	send_data[BasicRouterResponse::type_key] = BasicRouterResponse::object_key;
	return sendJson(prepareObject(send_data));
}

template<class RespT>
sf::Socket::Status BasicRouter<RespT>::send(const TransferableAction *action)
{
	using namespace std::string_literals;
	if (!action)
		return sf::Socket::Status::Error;
	Transferable::TransferJson send_data = action->requestData();
	send_data[BasicRouterResponse::type_key]  = BasicRouterResponse::action_key;
	if (action->m_author_id)
		send_data[BasicRouterResponse::author_id_key] = *action->m_author_id;
	else
		send_data[BasicRouterResponse::author_id_key] = nullptr;
	if (action->m_contributor_id)
		send_data[BasicRouterResponse::author_id_key] = *action->m_contributor_id;
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
