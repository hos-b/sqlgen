#ifndef SQLGEN_COL_HPP_
#define SQLGEN_COL_HPP_

#include <rfl.hpp>

namespace sqlgen {

template <rfl::internal::StringLiteral _name>
struct Col {};

template <rfl::internal::StringLiteral _name>
const auto col = Col<_name>{};

}  // namespace sqlgen

#endif
