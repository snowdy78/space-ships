#pragma once

#include <initializer_list>
#include <map>
#include <memory>
#include <type_traits>
#include <variant>
#include "game/GameManager.hpp"
#include "coop/TransferableAction.hpp"
#include "decl.hpp"
#include "game/GameObject.hpp"
#include "game/actions/AbstractAction.hpp"

template<class Action>
concept ControlsSchemeActionConcept = std::is_base_of_v<TransferableAction, Action>
									  && !std::is_same_v<Action, TransferableAction> && !std::is_abstract_v<Action>;
template<class K, class AcceptedKeyTypes, class P, class... PropsTypes>
concept SchemeBindingPropsKeysConcept = is_any_same_of_v<P, type_list<PropsTypes...>> && is_any_same_of_v<K, AcceptedKeyTypes>;

template<BindingSeparatorConcept BindingSepE, class KeyIdT, class AcceptedKeyTypes, class... PropsTypes>
class ControlsScheme
{
public:
	template<class K>
	struct binding_predicate;
	class binding;
	class binding_pointer;
	using action_t = TransferableAction;
	using action_props = TransferableActionProps;
	using string_t = sf::String;
	template<class T>
	using container = std::vector<T>;

private:
	template<class ActionT>
	struct action_type
	{
		action_type() = default;
	};
	template<class MapK, class V>
	using map_t				 = std::map<MapK, V>;
	using bindings_container = container<binding_pointer>;
	using mapped_items		 = map_t<BindingSepE, bindings_container>;

public:
	using value_type					= binding;
	using bindings_iterator				= typename bindings_container::iterator;
	using bindings_const_iterator		= typename bindings_container::const_iterator;
	using classification_iterator		= typename mapped_items::iterator;
	using classification_const_iterator = typename mapped_items::const_iterator;
	using InitializerMap
		= std::initializer_list<std::pair<BindingSepE, std::initializer_list<typename bindings_container::value_type>>>;
	struct classification;

	template<class ActionT, class PropsT, class KeyT>
	static binding_pointer create(const KeyT &key, const string_t &name, const PropsT &props);
	ControlsScheme(const InitializerMap &map);
	const bindings_container &classified(BindingSepE key) const;
	classification find(BindingSepE key) const;

private:
	mapped_items m_controls_scheme;
};

///
// Implementation of ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...> //
///

template<BindingSeparatorConcept B, class K, class A, class... PTs>
template<class ActionT, class PropsT, class KeyT>
typename ControlsScheme<B, K, A, PTs...>::binding_pointer
ControlsScheme<B, K, A, PTs...>::create(const KeyT &key, const string_t &name, const PropsT &props)
{
	return binding_pointer{ new binding(action_type<ActionT>(), key, name, props) };
}

template<BindingSeparatorConcept BindingSepE, class KeyIdT, class AcceptedKeyTypes, class... PropsTypes>
ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...>::ControlsScheme(const InitializerMap &map)
{
	for (auto &i: map)
	{
		m_controls_scheme.insert(
			{
				i.first,
				{ i.second.begin(), i.second.end() }
		 }
		);
	}
}

template<BindingSeparatorConcept B, class K, class A, class... PTs>
const typename ControlsScheme<B, K, A, PTs...>::bindings_container &
ControlsScheme<B, K, A, PTs...>::classified(B key) const
{
	return m_controls_scheme.at(key);
}

template<BindingSeparatorConcept B, class K, class A, class... PTs>
typename ControlsScheme<B, K, A, PTs...>::classification ControlsScheme<B, K, A, PTs...>::find(B key) const
{
	return classified(key);
}
