#pragma once

#include <optional>
#include "Transferable.hpp"
#include "game/actions/AbstractAction.hpp"

class TransferableActionProps
{
public:
	template<class T>
	using SmartPtr	 = std::weak_ptr<T>;
	template<class T>
	using Optional = std::optional<T>;
	template<class T>
	using OptionalPtr = Optional<SmartPtr<T>>;
	using ObjectPtr	 = SmartPtr<GameObject>;
	using OptionalObjectPtr = Optional<ObjectPtr>;
	using OptionalId = Optional<size_t>;
	using JsonType	 = rn::Json;
	OptionalObjectPtr author;
	OptionalObjectPtr contributor;
	JsonType props;
	TransferableActionProps(OptionalObjectPtr author = std::nullopt, OptionalObjectPtr contributor = std::nullopt, JsonType props = {});
	/**
	 * return true if author has value and author not expired
	 * @return 
	 */
	bool authorExist() const;
	/**
	 * return true if contributor has value and contributor not expired
	 * @return
	 */
	bool contributorExist() const;
	template<class T>
	OptionalPtr<T> castAuthor() const;
	template<class T>
	OptionalPtr<T> castContributor() const;
};
using ActionProps = TransferableActionProps;
class TransferableAction : public AbstractAction, public Transferable
{
protected:
	template<class T>
	static size_t identify();

public:
	template<class T>
	using SmartPtr	 = std::weak_ptr<T>;
	template<class T>
	using Optional = std::optional<T>;
	template<class T>
	using OptionalPtr = Optional<SmartPtr<T>>;
	using ObjectPtr	 = SmartPtr<GameObject>;
	using OptionalObjectPtr = Optional<ObjectPtr>;
	using OptionalId = Optional<size_t>;
	TransferableAction(const TransferableActionProps &props = {});
	OptionalObjectPtr getAuthor() const;
	OptionalObjectPtr getContributor() const;
	OptionalId getAuthorId() const;
	OptionalId getContributorId() const;
	bool playable() const override;

private:
	OptionalObjectPtr m_author_ptr{ std::nullopt };
	OptionalObjectPtr m_contributor_ptr{ std::nullopt };
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
	TransferableActionFabric() = default;
public:
	using create_action_func = std::function<std::unique_ptr<TransferableAction>(const TransferableActionProps &)>;

	static TransferableActionFabric &instance();

	const create_action_func &get(size_t id);
	template<class T>
	size_t push();
	void erase(size_t id);
	void clear();
private:
	std::unordered_map<size_t, create_action_func> transfer_actions{};
	size_t id_encounter		   = 0;
};

template<class T>
size_t TransferableActionFabric::push()
{
#ifdef SPACE_SHIP_DEBUG
	T::name = typeid(T).name();
#endif
	transfer_actions.emplace(
		id_encounter, [](const TransferableActionProps &props) -> std::unique_ptr<TransferableAction> {
			return std::unique_ptr<TransferableAction>{ new T(props) };
		}
	);
	return id_encounter++;
}

template<class T>
TransferableActionProps::OptionalPtr<T> TransferableActionProps::castAuthor() const
{
	if (authorExist())
		return std::dynamic_pointer_cast<T>(author->lock());
	return std::nullopt;
}

template<class T>
TransferableActionProps::OptionalPtr<T> TransferableActionProps::castContributor() const
{
	if (contributorExist())
		return std::dynamic_pointer_cast<T>(contributor->lock());
	return std::nullopt;
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
AbstractAction *TransferActionBase<T>::copy() const
{
	return new T({ getAuthor(), getContributor(), toJson() });
}
