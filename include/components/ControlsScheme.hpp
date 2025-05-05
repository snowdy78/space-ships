#pragma once

#include <initializer_list>
#include <map>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <variant>
#include "../game/GameGlobals.hpp"
#include "coop/TransferableAction.hpp"
#include "decl.hpp"
#include "game/GameObject.hpp"
#include "game/actions/AbstractAction.hpp"


template<class Action>
concept ControlsSchemeActionConcept = std::is_base_of_v<TransferableAction, Action>
									  && !std::is_same_v<Action, TransferableAction> && !std::is_abstract_v<Action>;

template<BindingSeparatorConcept BindingSepE, class KeyIdT, class AcceptedKeyTypes, class... PropsTypes>
class ControlsScheme
{
	using action_t = TransferableAction;
	using string_t = sf::String;
	template<class T>
	using container = std::vector<T>;

	template<class ActionT>
	struct action_type
	{
		static action_type<ActionT> create()
		{
			return action_type<ActionT>{};
		}
		action_type() = default;
	};
public:
	class scheme_binding
	{
		const std::type_info &key_type_info;
		const std::type_info &action_type_info;
		const std::function<TransferableAction *(GameObject *, GameObject *, const rn::Json &)> create_action = []() {
			return nullptr;
		};

	public:
		template<ControlsSchemeActionConcept Action, class KeyT, class PropsT>
		scheme_binding(const action_type<Action> &type, const KeyT key, const string_t &name, const PropsT &props)
			requires(is_any_of<KeyT, AcceptedKeyTypes>)
			: key(key),
			  name(name),
			  m_props(props),
			  key_type_info(typeid(KeyT)),
			  create_action([](GameObject *a, GameObject *b, const rn::Json &c) {
				  return new Action(a, b, c);
			  }),
			  action_type_info(typeid(Action))
		{
		}
		const KeyIdT key;
		const string_t name;
		const std::variant<PropsTypes...> m_props{};

		virtual TransferableAction *
		createAction(GameObject *a = nullptr, GameObject *b = nullptr, const rn::Json &c = {}) const
		{
			return create_action(a, b, c);
		}
		const std::type_info &getActionTypeInfo() const
		{
			return action_type_info;
		}

		template<class KeyT>
		bool keyIs() const
			requires(is_any_of<KeyT, AcceptedKeyTypes>)
		{
			return getKeyTypeInfo() == typeid(KeyT);
		}
		template<class PropsT>
		bool propsIs() const
		{
			return std::holds_alternative<PropsT>(m_props);
		}
		void pushAction(GameObject *a = nullptr, GameObject *b = nullptr, const rn::Json &c = {}) const
		{
			if (GameGlobals::exist())
				GameGlobals::instance().action_manager.addToTop(std::unique_ptr<AbstractAction>(createAction(a, b, c)));
		}
		template<class K, class P, class... Args>
		void pushIfReleased(
			std::function<bool(K)> f, std::function<rn::Json(const P &)> props_to_json, const Args &...args
		) const
		{
			if (this->released<K, P>(f))
			{
				auto &props = std::get<P>(m_props);
				this->pushAction(args..., props_to_json(props));
			}
		}
		template<class K, class P>
		void pushIfReleased(std::function<bool(K)> f, GameObject *a = nullptr, GameObject *b = nullptr, const rn::Json &c = {}) const
		{
			if (this->released<K, P>(f))
				this->pushAction(a, b, c);
		}
		const std::type_info &getKeyTypeInfo() const
		{
			return key_type_info;
		}
		template<class KeyT>
		KeyT as() const
			requires(is_any_of<KeyT, AcceptedKeyTypes>)
		{
			return static_cast<KeyT>(key);
		}
		template<ControlsSchemeActionConcept Action>
		bool actionIs() const
		{
			return getActionTypeInfo() == typeid(Action);
		}
		template<class K, class P>
		bool released(const std::function<bool(K)> &f) const
		{
			return keyIs<K>() && propsIs<P>() && f(static_cast<K>(key));
		}
		bool operator==(const scheme_binding &other) const
		{
			return getKeyTypeInfo() == other.getKeyTypeInfo() && key == other.key && name == other.name
				   && getActionTypeInfo() == other.getActionTypeInfo();
		}
		bool operator!=(const scheme_binding &other) const
		{
			return !operator==(other);
		}
	};

private:
	class binding_pointer
	{
		std::unique_ptr<scheme_binding> item{ nullptr };

	public:
		binding_pointer(scheme_binding *&&item)
			: item(std::move(item))
		{
		}
		binding_pointer(const binding_pointer &p)
		{
			if (p.item)
				item.reset(new scheme_binding(*p.item));
		}
		const scheme_binding &operator*() const
		{
			return *item;
		}
		const scheme_binding *operator->() const
		{
			return item.get();
		}
		bool operator==(const binding_pointer &other) const
		{
			return *item == *other.item;
		}
		bool operator!=(const binding_pointer &other) const
		{
			return !operator==(other);
		}
		binding_pointer &operator=(const binding_pointer &p)
		{
			if (this != &p)
			{
				if (p.item)
					item.reset(new scheme_binding(*p.item));
			}
			return *this;
		}
	};
	template<class MapK, class V>
	using map_t				 = std::map<MapK, V>;
	using bindings_container = container<binding_pointer>;
	using mapped_items		 = map_t<BindingSepE, bindings_container>;

public:
	using value_type	 = scheme_binding;
	using iterator		 = bindings_container::iterator;
	using const_iterator = bindings_container::const_iterator;

	using Binding = scheme_binding;
	using InitializerMap
		= std::initializer_list<std::pair<BindingSepE, std::initializer_list<typename bindings_container::value_type>>>;
	template<class ActionT, class KeyT, class PropsT>
	static scheme_binding *create(const KeyT &key, const string_t &name, const PropsT &props)
	{
		return new Binding(action_type<ActionT>::create(), key, name, props);
	}
	ControlsScheme(const InitializerMap &map)
	{
		for (auto &i: map)
			m_controls_scheme[i.first] = std::move(bindings_container(i.second.begin(), i.second.end()));
	}
	const bindings_container &classified(BindingSepE key) const
	{
		return m_controls_scheme.at(key);
	}

private:
	mapped_items m_controls_scheme;
};
