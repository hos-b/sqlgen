#ifndef SQLGEN_COL_HPP_
#define SQLGEN_COL_HPP_

#include <rfl.hpp>

#include "transpilation/Desc.hpp"

namespace sqlgen {

template <rfl::internal::StringLiteral _name>
struct Col {
  using Name = rfl::Literal<_name>;

  /// Signals to order_by that this column is to be sorted in descending order.
  auto desc() const noexcept { return transpilation::Desc<Col<_name>>{}; }
};

template <rfl::internal::StringLiteral _name>
const auto col = Col<_name>{};

}  // namespace sqlgen

#endif
