#ifndef SQLGEN_TRANSPILATION_CONDITION_HPP_
#define SQLGEN_TRANSPILATION_CONDITION_HPP_

#include <type_traits>

#include "conditions.hpp"

namespace sqlgen::transpilation {

template <class _ConditionType>
struct Condition {
  using ConditionType = _ConditionType;
  ConditionType condition;
};

template <class T>
auto make_condition(T&& _t) {
  return Condition<std::remove_cvref_t<T>>{.condition = _t};
}

template <class C1, class C2>
auto operator&&(const Condition<C1>& _cond1, const Condition<C2>& _cond2) {
  return make_condition(conditions::And<C1, C2>{.cond1 = _cond1.condition,
                                                .cond2 = _cond2.condition});
}

template <class C1, class C2>
auto operator||(const Condition<C1>& _cond1, const Condition<C2>& _cond2) {
  return make_condition(conditions::Or<C1, C2>{.cond1 = _cond1.condition,
                                               .cond2 = _cond2.condition});
}

}  // namespace sqlgen::transpilation

#endif
