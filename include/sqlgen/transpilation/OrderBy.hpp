#ifndef SQLGEN_TRANSPILATION_ORDERBY_HPP_
#define SQLGEN_TRANSPILATION_ORDERBY_HPP_

#include <rfl.hpp>
#include <type_traits>
#include <vector>

#include "../col.hpp"
#include "Desc.hpp"
#include "all_columns_exist.hpp"

namespace sqlgen::transpilation {

template <class _ColType>
struct OrderByWrapper;

template <rfl::internal::StringLiteral _name>
struct OrderByWrapper<Col<_name>> {
  using ColType = Col<_name>;
  constexpr static bool desc = false;
};

template <rfl::internal::StringLiteral _name>
struct OrderByWrapper<Desc<Col<_name>>> {
  using ColType = Col<_name>;
  constexpr static bool desc = true;
};

template <class... WrapperTypes>
struct OrderBy {};

template <class T, class... ColTypes>
auto make_order_by() {
  static_assert(
      all_columns_exist<T, typename OrderByWrapper<ColTypes>::ColType...>(),
      "All columns must exist.");
  return OrderBy<OrderByWrapper<ColTypes>...>{};
}

template <class T, class... ColTypes>
using order_by_t =
    std::invoke_result_t<decltype(make_order_by<T, ColTypes...>)>;

}  // namespace sqlgen::transpilation

#endif
