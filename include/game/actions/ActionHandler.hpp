#pragma once

#include "coop/TransferableAction.hpp"
#include "decl.hpp"

template<class T>
concept HandlerActionConcept = std::is_base_of_v<TransferActionBase<T>, T> && !std::is_abstract_v<T>;

class ActionHandler
{
public:
	using handle_func_type = std::function<void()>;
	ActionHandler(size_t action_id, handle_func_type func)
		: m_handler(std::move(func)),
		  m_action_id(action_id)
	{
	}
	size_t getActionId() const
	{
		return m_action_id;
	}
	virtual ~ActionHandler() = default;
	void operator()() const
	{
		m_handler();
	}
private:
	handle_func_type m_handler;
	size_t m_action_id;
};
