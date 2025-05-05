#ifndef SQLGEN_WHERE_HPP_
#define SQLGEN_WHERE_HPP_

#include <type_traits>

#include "Result.hpp"
#include "read.hpp"
#include "transpilation/Limit.hpp"
#include "transpilation/value_t.hpp"

namespace sqlgen {

template <class ConditionType>
struct Where {
  ConditionType condition;
};

template <class ContainerType, class WhereType, class OrderByType,
          class LimitType, class ConditionType>
auto operator|(const Read<ContainerType, WhereType, OrderByType, LimitType>& _r,
               const Where<ConditionType>& _where) {
  static_assert(std::is_same_v<WhereType, Nothing>,
                "You cannot call where(...) twice (but you can apply more "
                "than one condition by combining them with && or ||).");
  static_assert(std::is_same_v<OrderByType, Nothing>,
                "You must call order_by(...) after where(...).");
  static_assert(std::is_same_v<LimitType, Nothing>,
                "You must call limit(...) after where(...).");
  return Read<ContainerType, ConditionType, OrderByType, LimitType>{
      .where_ = _where.condition};
}

template <class ConditionType>
inline auto where(const ConditionType& _cond) {
  return Where<std::remove_cvref_t<ConditionType>>{.condition = _cond};
};

}  // namespace sqlgen

#endif
