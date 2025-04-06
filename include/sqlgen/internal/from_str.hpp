#ifndef SQLGEN_INTERNAL_FROM_STR_HPP_
#define SQLGEN_INTERNAL_FROM_STR_HPP_

#include <optional>
#include <string>
#include <type_traits>

#include "../Result.hpp"
#include "../parsing/Parser.hpp"

namespace sqlgen::internal {

template <class T>
Result<T> from_str(const std::optional<std::string>& _str) {
  using Type = std::remove_cvref_t<T>;
  return Parser<Type>::read(_str);
}

}  // namespace sqlgen::internal

#endif
