#ifndef SQLGEN_INTERNAL_TO_STR_VEC_HPP_
#define SQLGEN_INTERNAL_TO_STR_VEC_HPP_

#include <optional>
#include <rfl.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "to_str.hpp"

namespace sqlgen::internal {

template <class T>
std::vector<std::optional<std::string>> to_str_vec(const T& _t) {
  return rfl::apply(
      [](auto... _ptrs) {
        return std::vector<std::optional<std::string>>({to_str(*_ptrs)...});
      },
      rfl::to_view(_t).values());
}

}  // namespace sqlgen::internal

#endif
