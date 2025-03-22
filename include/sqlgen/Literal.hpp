#ifndef SQLGEN_LITERAL_HPP_
#define SQLGEN_LITERAL_HPP_

#include <rfl.hpp>

namespace sqlgen {

template <rfl::internal::StringLiteral _name>
using Literal = rfl::Literal<_name>;

};  // namespace sqlgen

#endif

