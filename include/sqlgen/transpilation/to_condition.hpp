#ifndef SQLGEN_TRANSPILATION_TO_CONDITION_HPP_
#define SQLGEN_TRANSPILATION_TO_CONDITION_HPP_

#include <type_traits>
#include <vector>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../dynamic/Condition.hpp"
#include "Condition.hpp"
#include "conditions.hpp"

namespace sqlgen::transpilation {

template <class T, class ConditionType>
struct ToCondition;

template <class T, class CondType>
struct ToCondition<T, Condition<CondType>> {
  dynamic::Condition operator()(const Condition<CondType>& _cond) const {
    return ToCondition<T, std::remove_cvref_t<CondType>>{}(_cond.condition);
  }
};

template <class T, class CondType1, class CondType2>
struct ToCondition<T, conditions::And<CondType1, CondType2>> {
  dynamic::Condition operator()(
      const conditions::And<CondType1, CondType2>& _cond) const {
    return dynamic::Condition{
        .val = dynamic::Condition::And{
            .cond1 = Ref<dynamic::Condition>::make(
                ToCondition<T, std::remove_cvref_t<CondType1>>{}(_cond.cond1)),
            .cond2 = Ref<dynamic::Condition>::make(
                ToCondition<T, std::remove_cvref_t<CondType2>>{}(_cond.cond2)),
        }};
  }
};

template <class T, class CondType1, class CondType2>
struct ToCondition<T, conditions::Or<CondType1, CondType2>> {
  dynamic::Condition operator()(
      const conditions::Or<CondType1, CondType2>& _cond) const {
    return dynamic::Condition{
        .val = dynamic::Condition::Or{
            .cond1 = Ref<dynamic::Condition>::make(
                ToCondition<T, std::remove_cvref_t<CondType1>>{}(_cond.cond1)),
            .cond2 = Ref<dynamic::Condition>::make(
                ToCondition<T, std::remove_cvref_t<CondType2>>{}(_cond.cond2)),
        }};
  }
};

template <class T, class ConditionType>
dynamic::Condition to_condition(const ConditionType& _cond) {
  return ToCondition<T, std::remove_cvref_t<ConditionType>>{}(_cond);
}

}  // namespace sqlgen::transpilation

#endif
