#pragma once

#include <optional>
#include "Transferable.hpp"
#include "game/actions/AbstractAction.hpp"

class TransferableActionProps
{
public:
	GameObject *author;
	GameObject *contributor;
	rn::Json props;
	TransferableActionProps(GameObject *author = nullptr, GameObject *contributor = nullptr, rn::Json props = {});
};

class TransferableAction : public AbstractAction, public Transferable
{
	template<class T>
	friend class BasicRouter;
protected:
	template<class T>
	static size_t identify();

public:
	TransferableAction(const TransferableActionProps &props = {});
	GameObject *getAuthor() const;
	GameObject *getContributor() const;

private:
	GameObject *m_author_ptr;
	GameObject *m_contributor_ptr;
	std::optional<size_t> m_author_id{std::nullopt};
	std::optional<size_t> m_contributor_id{std::nullopt};
};



template<class T>
struct TransferActionBase : TransferableAction
{
	inline static const size_t identifier = identify<T>();
	TransferActionBase(const TransferableActionProps &attrs);
	~TransferActionBase() override;
	rn::Json toJson() const override;
	TransferJson requestData() const override;
	AbstractAction *copy() const override;

private:
	using TransferableAction::identify;
};

class TransferableActionFabric
{
	using create_action_func = std::function<
		std::unique_ptr<TransferableAction>(const TransferableActionProps &)>;

	std::unordered_map<size_t, create_action_func> transfer_actions{};
	size_t id_encounter		   = 0;
	TransferableActionFabric() = default;

public:
	static TransferableActionFabric &instance();

	const create_action_func &get(size_t id);
	template<class T>
	size_t push();
	void erase(size_t id);
	void clear();
};

template<class T>
size_t TransferableActionFabric::push()
{
	transfer_actions.emplace(
		id_encounter,
		[](const TransferableActionProps &props) -> std::unique_ptr<TransferableAction> {
			return std::unique_ptr<TransferableAction>{ new T(props) };
		}
	);
	return id_encounter++;
}

template<class T>
size_t TransferableAction::identify()
{
	return TransferableActionFabric::instance().push<T>();
}

template<class T>
TransferActionBase<T>::TransferActionBase(const TransferableActionProps &attrs)
	: TransferableAction(attrs)
{

}

template<class T>
TransferActionBase<T>::~TransferActionBase() = default;

template<class T>
rn::Json TransferActionBase<T>::toJson() const
{
	return {};
}

template<class T>
Transferable::TransferJson TransferActionBase<T>::requestData() const
{
	return { identifier, toJson() };
}

template<class T>
AbstractAction * TransferActionBase<T>::copy() const
{
	return new T({ getAuthor(), getContributor(), toJson() });
}
