#pragma once

#include <optional>
#include "Transferable.hpp"
#include "game/actions/AbstractAction.hpp"


template<class T>
concept TransferActionConcept
	= is_fabric_type_v<TransferableAction, T>
	  && requires(T *ptr, T value, GameObject *author, GameObject *contributor, const rn::Json &props) {
			 ptr = new T(author, contributor, props);
		 };

class TransferableAction : public AbstractAction, public Transferable
{
	template<class T>
	friend class BasicRouter;
protected:
	template<TransferActionConcept T>
	static size_t identify();

public:
	TransferableAction(GameObject *author, GameObject *contributor, const rn::Json &props);

private:
	std::optional<size_t> author{std::nullopt};
	std::optional<size_t> contributor{std::nullopt};
};


class TransferableActionFabric
{
	using create_action_func = std::function<
		std::unique_ptr<TransferableAction>(GameObject *author, GameObject *contributor, const rn::Json &props)>;

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
		[](GameObject *author, GameObject *contributor, const rn::Json &props) -> std::unique_ptr<TransferableAction> {
			return std::unique_ptr<TransferableAction>{ new T(author, contributor, props) };
		}
	);
	return id_encounter++;
}

template<TransferActionConcept T>
size_t TransferableAction::identify()
{
	return TransferableActionFabric::instance().push<T>();
}
