#pragma once
#include "ControlsSchemeDecl.hpp"
/**
 * @brief classification
 * In ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...>
 * @tparam BindingSepE
 * @tparam KeyIdT
 * @tparam AcceptedKeyTypes
 * @tparam PropsTypes
 */
template<BindingSeparatorConcept BindingSepE, class KeyIdT, class AcceptedKeyTypes, class... PropsTypes>
struct ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...>::classification
{
	classification(const bindings_container &bindings)
		: m_bindings(bindings)
	{
	}
	bindings_const_iterator begin()
	{
		return m_bindings.begin();
	}
	bindings_const_iterator end()
	{
		return m_bindings.end();
	}
	template<class P, class K>
		requires SchemeBindingPropsKeysConcept<K, AcceptedKeyTypes, P, PropsTypes...>
	bindings_const_iterator ifOneOf(const binding_predicate<K> &f) const
	{
		return std::find_if(m_bindings.begin(), m_bindings.end(), [&f](const binding_pointer &binding) {
			return binding && binding->template is<P, K>(f);
		});
	}
	template<class P, class K>
		requires SchemeBindingPropsKeysConcept<K, AcceptedKeyTypes, P, PropsTypes...>
	bool oneOf(const binding_predicate<K> &f) const
	{
		return this->ifOneOf<P, K>(f) != m_bindings.end();
	}

	template<class P, class K>
		requires SchemeBindingPropsKeysConcept<K, AcceptedKeyTypes, P, PropsTypes...>
	void pushActionIfOneOf(
		const binding_predicate<K> &f, const std::function<void(const P &)> &prepare_props, const action_props &props
	) const
	{
		auto bind = this->ifOneOf<P, K>(f);
		if (bind != m_bindings.end())
		{
			prepare_props(std::get<P>(*bind->m_props));
			(*bind)->pushAction(props);
		}
	}
	template<class P, class K>
		requires SchemeBindingPropsKeysConcept<K, AcceptedKeyTypes, P, PropsTypes...>
	void pushActionIfOneOf(
		const binding_predicate<K> &f, const action_props &props
	) const
	{
		auto bind = this->ifOneOf<P, K>(f);
		if (bind != m_bindings.end())
		{
			(*bind)->pushAction(props);
		}
	}
	template<class P, class K>
		requires SchemeBindingPropsKeysConcept<K, AcceptedKeyTypes, P, PropsTypes...>
	void forEachPushAction(
		const binding_predicate<K> &f, const std::function<const P &> &prepare_props, const action_props &props
	) const
	{
		for (auto &binding_ptr: m_bindings)
			if (binding_ptr)
				binding_ptr->template pushActionIf<P, K>(f, prepare_props, props);
	}

	/**
	 * @brief pushes the last right action 
	 * @tparam P 
	 * @tparam K 
	 * @param f 
	 * @param predicate 
	 * @param props 
	 */
	template<class P, class K>
		requires SchemeBindingPropsKeysConcept<K, AcceptedKeyTypes, P, PropsTypes...>
	void pushAsOneAction(
		const binding_predicate<K> &f, const std::function<void(const P &)> &predicate, const action_props &props
	) const
	{
		const binding_pointer *pointer{nullptr};
		for (auto &binding_ptr: m_bindings)
		{
			if (binding_ptr && binding_ptr->template is<P, K>(f))
			{
				pointer = &binding_ptr;
				predicate(binding_ptr->template propsAs<P>());
			}
		}
		if (pointer)
			(*pointer)->pushAction(props);
	}

private:
	const bindings_container &m_bindings;
};
