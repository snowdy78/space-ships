#pragma once

#include "Transferable.hpp"
#include "game/actions/AbstractAction.hpp"


template<class T>
concept TransferActionConcept
	= is_fabric_type_v<TransferableAction, T>
	  && requires(T *ptr, T value, GameObject *author, GameObject *target, const rn::Json &props) {
			 ptr = new T(author, target, props);
		 };

class TransferableAction : public AbstractAction, public Transferable
{
protected:
	template<TransferActionConcept T>
	static size_t identify();

public:
	TransferableAction(GameObject *author, GameObject *target, const rn::Json &props);
};


class TransferableActionFabric
{
	using create_action_func = std::function<
		std::unique_ptr<TransferableAction>(GameObject *author, GameObject *target, const rn::Json &props)>;

	std::unordered_map<size_t, create_action_func> transfer_actions{};
	size_t id_encounter		   = 0;
	TransferableActionFabric() = default;

public:
	static TransferableActionFabric &instance();

	const create_action_func &get(size_t id);
	template<TransferActionConcept T>
	size_t push();
	void erase(size_t id);
	void clear();
};

template<TransferActionConcept T>
size_t TransferableActionFabric::push()
{
	transfer_actions.emplace(
		id_encounter,
		[](GameObject *author, GameObject *target, const rn::Json &props) -> std::unique_ptr<TransferableAction> {
			return std::unique_ptr<TransferableAction>{ new T(author, target, props) };
		}
	);
	return id_encounter++;
}

template<TransferActionConcept T>
size_t TransferableAction::identify()
{
	return TransferableActionFabric::instance().push<T>();
}
