#ifndef SQLGEN_TIMESTAMP_HPP_
#define SQLGEN_TIMESTAMP_HPP_

#include <rfl.hpp>

namespace sqlgen {

template <rfl::internal::StringLiteral _format>
using Timestamp = rfl::Timestamp<_format>;

};  // namespace sqlgen

#endif

