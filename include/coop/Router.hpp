#pragma once

#include <variant>
#include <optional>
#include "decl.hpp"
#include "TransferableObject.hpp"
#include "TransferableAction.hpp"

class BasicRouterResponse
{
    std::unique_ptr<Transferable::TransferJson> transfer_json = nullptr;
    sf::Socket::Status m_status = sf::Socket::Done;
    BasicRouterResponse(const rn::Json &data_json = {});
    BasicRouterResponse(const sf::Socket::Status &status);
    template<class RespT>
    friend class BasicRouter;

public:
    struct TransferType
	{
		inline static constexpr std::string object = "object";
		inline static constexpr std::string action = "action";
	};
    
    bool success();
    sf::Socket::Status status();
    bool is_unknown() const;
    bool is_action() const;
    bool is_object() const;
    std::unique_ptr<TransferableObject> object() const;
    std::unique_ptr<TransferableAction> action() const;
    const Transferable::TransferJson &json() const;
};

template<class RespT>
class BasicRouter
{
public:
    using Response = RespT;
    using StatusOrJson = std::variant<sf::Socket::Status, rn::Json>;

    BasicRouter() = default;

    sf::Socket::Status send(const TransferableObject *data);
	sf::Socket::Status send(TransferableAction *action);
	Response receive();
protected:
    virtual sf::Socket::Status sendJson(const rn::Json &) = 0;
    virtual StatusOrJson receiveJson() = 0;
};

template<class RespT>
sf::Socket::Status BasicRouter<RespT>::send(const TransferableObject *data) 
{
    auto send_data	  = data->toJson();
	send_data["type"] = Response::TransferType::object;
	return sendJson(send_data);
}

template<class RespT>
sf::Socket::Status BasicRouter<RespT>::send(TransferableAction *action)
{
    using namespace std::string_literals;
	if (!action)
		return sf::Socket::Status::Error;
	rn::Json send_data = action->toJson();
	send_data["type"]  = Response::TransferType::action;
	if (action->author)
		send_data["author_id"] = *action->author;
	else
		send_data["author_id"] = nullptr;
	if (action->contributor)
		send_data["target_id"] = *action->contributor;
	else
		send_data["target_id"] = nullptr;
	return sendJson(send_data);
}

template<class RespT>
typename BasicRouter<RespT>::Response BasicRouter<RespT>::receive() 
{
    auto json_or_status = receiveJson();
	if (std::holds_alternative<sf::Socket::Status>(json_or_status))
	{
		return std::get<sf::Socket::Status>(json_or_status);
	}

	auto &json = std::get<rn::Json>(json_or_status);
	if (!json.contains("type") || !json.at("type").is_string())
		throw incorrect_json_format("invalid json format");
	auto type = json.at("type");
	if (type != Response::TransferType::action && type != Response::TransferType::object)
		throw incorrect_json_format("invalid type name");
	return Response(json);
}
