#pragma once

#include "AbstractAction.hpp"
#include "ActionHandler.hpp"
#include "coop/Router.hpp"

class ActionManager : public rn::LogicalObject
{
	template<class T>
	using container_t		   = std::vector<T>;
	using value_type		   = std::unique_ptr<AbstractAction>;
	using handler_type		   = std::weak_ptr<ActionHandler>;
	using actions_container_t  = container_t<value_type>;
	using handlers_container_t = container_t<handler_type>;

	struct Transfering
	{
		Transfering(TransferType type);

		void switch_off();
		void switch_on();
		void transfer_type(TransferType type);
		bool swiched_on() const;
		TransferType transfer_type() const;

	private:
		bool m_turn{ true };
		TransferType m_transfer_type;
	};
	void realizeActions();
	void transferAction(AbstractAction *action) const;
	template<class T>
	void appendInto(T &&v) noexcept;

public:
	ActionManager() = default;
	template<class ActionT, class... Args>
	void emplaceToTop(const Args &...args) noexcept;
	void addToTop(std::unique_ptr<AbstractAction> &&action);
	void receiveToTop(std::unique_ptr<TransferableAction> &&action);
	void setTransfering(TransferType type);
	bool isTransfering() const;
	void hook(const std::weak_ptr<ActionHandler> &handler);
	void clear();
	void start() override;
	void update() override;

private:
	actions_container_t m_actions;
	// for appending actions while action is performed
	actions_container_t m_stack;
	handlers_container_t m_handlers;
	bool m_start_loop = false;

	std::optional<Transfering> m_transfering = std::nullopt;
};

template<class T>
void ActionManager::appendInto(T &&v) noexcept
{
	T &&a = std::forward<T>(v);
	if (!m_start_loop)
		m_actions.push_back(std::forward<T>(a));
	else
		m_stack.push_back(std::forward<T>(a));
}

template<class ActionT, class... Args>
void ActionManager::emplaceToTop(const Args &...args) noexcept
{
	auto action = std::make_unique<ActionT>(args...);
	this->addToTop(std::move(action));
}
