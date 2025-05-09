#pragma once

#include "ControlsSchemeDecl.hpp"

/**
 * @brief binding_predicate
 * In ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...>
 *
 * @tparam BindingSepE
 * @tparam KeyIdT
 * @tparam AcceptedKeyTypes
 * @tparam PropsTypes
 * @tparam K
 */
template<BindingSeparatorConcept BindingSepE, class KeyIdT, class AcceptedKeyTypes, class... PropsTypes>
template<class K>
struct ControlsScheme<BindingSepE, KeyIdT, AcceptedKeyTypes, PropsTypes...>::binding_predicate : std::function<bool(K)>
{
	using base_t = std::function<bool(K)>;
	using std::function<bool(K)>::function;
	using func_ptr = bool (*)(K);
	binding_predicate(func_ptr predicate)
		: base_t([predicate](auto k) {
			  return predicate(k);
		  })
	{
	}
	using init_list_t			  = std::initializer_list<func_ptr>;
	using predicate_func_iterator = typename init_list_t::const_iterator;
	using predicate_func		  = bool (*)(
		 predicate_func_iterator, predicate_func_iterator, std::function<bool(const typename init_list_t::value_type &)>
	 );
	binding_predicate(predicate_func predicate, const init_list_t &predicates)
		: base_t([predicates, predicate](auto k) {
			  return predicate(predicates.begin(), predicates.end(), [&k](auto p) {
				  return p(k);
			  });
		  })
	{
	}
};
