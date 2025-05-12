#pragma once

#include <optional>
#include "Transferable.hpp"
#include "game/actions/AbstractAction.hpp"


template<class T>
class BaseTransferableAction;

class TransferableActionProps
{
public:
	GameObject *author;
	GameObject *contributor;
	rn::Json data;
	TransferableActionProps(
		GameObject *author = nullptr, GameObject *contributor = nullptr, const rn::Json &props = {}
	);
};

using TA_Props = TransferableActionProps;

template<class T>
concept TransferActionConcept
	= is_fabric_type_v<BaseTransferableAction<T>, T>
	  && requires(T *ptr, T value, GameObject *author, GameObject *contributor, const rn::Json &props) {
			 ptr = new T(TransferableActionProps{author, contributor, props});
		 };

class TransferableAction : public AbstractAction, public Transferable
{
	template<class T>
	friend class BasicRouter;
	template<class T>
	friend class BaseTransferableAction;
	template<class>
	constexpr static void identify()
	{
	}

	TransferableAction(const TransferableActionProps &props = {});

protected:
	using IdentifierT = void (*)();

private:
	std::optional<size_t> author{std::nullopt};
	std::optional<size_t> contributor{std::nullopt};
};

template<class T>
class BaseTransferableAction : public TransferableAction
{
	static bool emplaceToFabric();
	constexpr static IdentifierT cs_id	  = &identify<T>;
	inline static bool emplaced_to_fabric = emplaceToFabric();

public:
	BaseTransferableAction(const TransferableActionProps &props = {})
		: TransferableAction(props)
	{
	}
	static size_t id()
	{
		return reinterpret_cast<size_t>(cs_id);
	}
};

class TransferableActionFabric
{
	using create_action_func = std::function<
		std::unique_ptr<TransferableAction>(GameObject *author, GameObject *contributor, const rn::Json &props)>;

	std::unordered_map<size_t, create_action_func> transfer_actions{};
	TransferableActionFabric() = default;

public:
	static TransferableActionFabric &instance();

	const create_action_func &get(size_t id);
	template<TransferActionConcept T>
	void push();
	void erase(size_t id);
	void clear();
};

template<class T>
bool BaseTransferableAction<T>::emplaceToFabric()
{
	TransferableActionFabric::instance().push<T>();
	return true;
}

template<TransferActionConcept T>
void TransferableActionFabric::push()
{
	transfer_actions.emplace(
		T::id(),
		[](GameObject *author, GameObject *contributor, const rn::Json &props) -> std::unique_ptr<TransferableAction> {
			return std::unique_ptr<TransferableAction>{ new T(TransferableActionProps{author, contributor, props}) };
		}
	);
}
