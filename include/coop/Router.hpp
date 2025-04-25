#pragma once

#include <variant>
#include <optional>
#include "decl.hpp"
#include "Transferable.hpp"

class BasicRouterResponse
{
    Transferable::TransferJson transfer_json;
    BasicRouterResponse(const rn::Json &data_json = {});
    template<class RespT>
    friend class BasicRouter;

public:
    struct TransferType
	{
		inline static constexpr std::string object = "object";
		inline static constexpr std::string action = "action";
	};
    
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

	BasicRouter();

    virtual sf::Socket::Status sendObject(const TransferableObject *data) = 0;
	virtual sf::Socket::Status sendAction(std::optional<size_t> author_id, std::optional<size_t> target_id, TransferableAction *action) = 0;
	virtual std::variant<sf::Socket::Status, BasicRouter::Response> recieve() = 0;
protected:
    BasicRouter::Response createResponseBody(const rn::Json &data);
};
