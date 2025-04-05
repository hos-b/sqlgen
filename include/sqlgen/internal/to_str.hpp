#ifndef SQLGEN_INTERNAL_TO_STR_HPP_
#define SQLGEN_INTERNAL_TO_STR_HPP_

#include <optional>
#include <string>
#include <type_traits>

#include "../parsing/has_reflection_method.hpp"
#include "../parsing/is_nullable.hpp"

namespace sqlgen::internal {

template <class T>
std::optional<std::string> to_str(const T& _val) {
  using Type = std::remove_cvref_t<T>;
  if constexpr (parsing::is_nullable_v<Type>) {
    if (!_val) {
      return std::nullopt;
    } else {
      return to_str(*_val);
    }

  } else if constexpr (parsing::has_reflection_method<Type>) {
    return to_str(_val.reflection());

  } else if constexpr (std::is_same_v<Type, std::string>) {
    return _val;

  } else {
    return std::to_string(_val);
  }
}

}  // namespace sqlgen::internal

#endif
