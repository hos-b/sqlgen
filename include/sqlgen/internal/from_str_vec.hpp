#ifndef SQLGEN_INTERNAL_FROM_STR_VEC_HPP_
#define SQLGEN_INTERNAL_FROM_STR_VEC_HPP_

#include <optional>
#include <rfl.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../parsing/ViewReader.hpp"

namespace sqlgen::internal {

template <class T>
Result<T> from_str_vec(
    const std::vector<std::optional<std::string>>& _str_vec) {
  T t;
  auto view = rfl::to_view(t);
  auto view_reader = parsing::ViewReader<remove_cvref_t<decltype(view)>>(&view);
  const auto [err, num_fields_assigned] = view_reader.read(_str_vec);
  if (err) {
    return error(err->what());
  }
  return t;
}

}  // namespace sqlgen::internal

#endif
