#ifndef SQLGEN_INTERNAL_FROM_STR_VEC_HPP_
#define SQLGEN_INTERNAL_FROM_STR_VEC_HPP_

#include <optional>
#include <rfl.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "../Result.hpp"

namespace sqlgen::internal {

template <class T>
Result<T> from_str_vec(
    const std::vector<std::optional<std::string>>& _str_vec) {
  T t;
  const auto view = rfl::to_view(t);

  return t;
}

}  // namespace sqlgen::internal

#endif
