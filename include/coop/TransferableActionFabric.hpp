#pragma once

#include "decl.hpp"

class TransferableActionFabric
{
	using create_action_func = std::function<
		TransferableAction *(std::optional<GameObject *> author_id, std::optional<GameObject *> target_id, const rn::Json &props)>;

	std::unordered_map<size_t, create_action_func> transfer_actions{};
	size_t id_encounter		   = 0;
	TransferableActionFabric() = default;

public:
	static TransferableActionFabric &instance();

	const create_action_func &get(size_t id);
	template<class T>
	size_t
	push(std::optional<GameObject *> author_id = std::nullopt, std::optional<GameObject *> target_id = std::nullopt, const rn::Json &props = {});
	void erase(size_t id);
	void clear();
};

template<class T>
size_t TransferableActionFabric::push(std::optional<GameObject *> author_id, std::optional<GameObject *> target_id, const rn::Json &props)
{
	transfer_actions.emplace(
		id_encounter,
		[](std::optional<GameObject *> author_id, std::optional<GameObject *> target_id, const rn::Json &props) -> TransferableAction * {
			return new T(author_id, target_id, props);
		}
	);
	return id_encounter++;
}
