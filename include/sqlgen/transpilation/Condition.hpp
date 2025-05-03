#ifndef SQLGEN_TRANSPILATION_CONDITION_HPP_
#define SQLGEN_TRANSPILATION_CONDITION_HPP_

#include "conditions.hpp"

namespace sqlgen::transpilation {

template <class _ConditionType>
struct Condition {
  using ConditionType = _ConditionType;
  ConditionType condition;
};

template <class C1, class C2>
auto operator&&(const Condition<C1>& _cond1, const Condition<C2>& _cond2) {
  return Condition<conditions::And<C1, C2>>{
      .condition = conditions::And<C1, C2>{.cond1 = _cond1.condition,
                                           .cond2 = _cond2.condition}};
}

template <class C1, class C2>
auto operator||(const Condition<C1>& _cond1, const Condition<C2>& _cond2) {
  return Condition<conditions::Or<C1, C2>>{
      .condition = conditions::Or<C1, C2>{.cond1 = _cond1.condition,
                                          .cond2 = _cond2.condition}};
}

}  // namespace sqlgen::transpilation

#endif
