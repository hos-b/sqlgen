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
auto operator&&(const Condition<C1>& _lhs, const Condition<C2>& _rhs) {
  return Condition<conditions::And<C1, C2>>{
      .condition = conditions::And<C1, C2>{.lhs = _lhs.condition,
                                           .rhs = _rhs.condition}};
}

template <class C1, class C2>
auto operator||(const Condition<C1>& _lhs, const Condition<C2>& _rhs) {
  return Condition<conditions::Or<C1, C2>>{
      .condition =
          conditions::Or<C1, C2>{.lhs = _lhs.condition, .rhs = _rhs.condition}};
}

}  // namespace sqlgen::transpilation

#endif
