#pragma once
#include "ControlsSchemeDecl.hpp"

/**
 * @brief scheme_binding
 * In ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...>
 *
 * @tparam BindingSepE
 * @tparam KeyIdT
 * @tparam AcceptedKeyTypes
 * @tparam PropsTypes
 */
template<BindingSeparatorConcept BindingSepE, class KeyIdT, class AcceptedKeyTypes, class... PropsTypes>
class ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...>::binding
{

public:
	template<ControlsSchemeActionConcept Action, class KeyT, class PropsT>
		requires SchemeBindingPropsKeysConcept<KeyT, AcceptedKeyTypes, PropsT, PropsTypes...>
	binding(const action_type<Action> &type, const KeyT key, const string_t &name, const PropsT &props)
		requires(is_any_same_of_v<KeyT, AcceptedKeyTypes>)
		: m_key(key),
		  m_name(name),
		  m_props(props),
		  m_key_type_info(typeid(KeyT)),
		  m_action_type_info(typeid(Action)),
		  m_create_action([](const action_props &props) {
			  return new Action(props);
		  })
	{
	}
	binding(const binding &binding) = default;
	binding(binding &&) noexcept	= default;
	virtual ~binding()				= default;
	virtual TransferableAction *createAction(const action_props &props) const
	{
		return m_create_action(props);
	}
	const std::type_info &getActionTypeInfo() const
	{
		return m_action_type_info;
	}

	template<class KeyT>
		requires(is_any_same_of_v<KeyT, AcceptedKeyTypes>)
	bool keyIs() const
		requires(is_any_same_of_v<KeyT, AcceptedKeyTypes>)
	{
		return getKeyTypeInfo() == typeid(KeyT);
	}
	template<class PropsT>
		requires(is_any_same_of_v<PropsT, type_list<PropsTypes...>>)
	bool propsIs() const
	{
		return std::holds_alternative<PropsT>(m_props);
	}
	template<ControlsSchemeActionConcept Action>
	bool actionIs() const
	{
		return getActionTypeInfo() == typeid(Action);
	}
	void pushAction(const action_props &props) const
	{
		if (GameManager::exist())
			GameManager::instance().action_manager.addToTop(std::unique_ptr<AbstractAction>(createAction(props)));
	}
	template<class P, class K>
		requires SchemeBindingPropsKeysConcept<K, AcceptedKeyTypes, P, PropsTypes...>
	void pushActionIf(
		const binding_predicate<K> &f, const std::function<void(const P &)> &prepare_props, const action_props &props
	) const
	{
		if (this->is<P, K>(f))
		{
			prepare_props(std::get<P>(m_props));
			this->pushAction(props);
		}
	}
	template<class P, class K>
		requires SchemeBindingPropsKeysConcept<K, AcceptedKeyTypes, P, PropsTypes...>
	void pushActionIf(
		const binding_predicate<K> &f, const action_props &props
	) const
	{
		if (this->is<P, K>(f))
		{
			prepare_props(std::get<P>(m_props));
			this->pushAction(props);
		}
	}
	const std::type_info &getKeyTypeInfo() const
	{
		return m_key_type_info;
	}
	template<class KeyT>
		requires(is_any_same_of_v<KeyT, AcceptedKeyTypes>)
	KeyT keyAs() const
		requires(is_any_same_of_v<KeyT, AcceptedKeyTypes>)
	{
		return static_cast<KeyT>(m_key);
	}
	template<class PropsT>
		requires(is_any_same_of_v<PropsT, type_list<PropsTypes...>>)
	const PropsT &propsAs() const
	{
		return std::get<PropsT>(m_props);
	}
	template<class P, class K>
		requires SchemeBindingPropsKeysConcept<K, AcceptedKeyTypes, P, PropsTypes...>
	bool is(const binding_predicate<K> &f) const
	{
		return keyIs<K>() && propsIs<P>() && f(static_cast<K>(m_key));
	}
	bool operator==(const binding &other) const
	{
		return getKeyTypeInfo() == other.getKeyTypeInfo() && m_key == other.m_key && m_name == other.m_name
			   && getActionTypeInfo() == other.getActionTypeInfo();
	}
	bool operator!=(const binding &other) const
	{
		return !operator==(other);
	}
	binding &operator=(const binding &)		= default;
	binding &operator=(binding &&) noexcept = default;

public:
	const KeyIdT m_key;
	const string_t m_name;
	const std::variant<PropsTypes...> m_props{};

private:
	const std::type_info &m_key_type_info;
	const std::type_info &m_action_type_info;
	const std::function<TransferableAction *(const action_props &)> m_create_action = []() {
		return nullptr;
	};
};
