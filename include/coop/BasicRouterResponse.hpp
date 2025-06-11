#pragma once

#include "decl.hpp"
#include <optional>


class BasicRouterResponse
{
	std::optional<rn::Json> m_response_data = std::nullopt;
	sf::Socket::Status m_status				= sf::Socket::Done;
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
	static constexpr const char *object_key			= "object";
	static constexpr const char *action_key			= "action";
	std::optional<size_t> id() const;
	std::optional<rn::Json> data() const;
	bool success() const;
	sf::Socket::Status status() const;
	bool is_unknown() const;
	bool is_action() const;
	bool is_object() const;
	std::unique_ptr<TransferableObject> object() const;
	std::unique_ptr<TransferableAction> action(const SpaceField &) const;
	const rn::Json &json() const;
};
