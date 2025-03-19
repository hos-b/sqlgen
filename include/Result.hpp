#ifndef SQLGEN_RESULT_HPP_
#define SQLGEN_RESULT_HPP_

#include <rfl.hpp>

namespace sqlgen {

using Error = rfl::Error;

using Nothing = rfl::Nothing;

template <class T>
using Result = rfl::Result<T>;

};  // namespace sqlgen

#endif

