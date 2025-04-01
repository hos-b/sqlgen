#ifndef SQLGEN_PARSING_GET_SCHEMA_HPP_
#define SQLGEN_PARSING_GET_SCHEMA_HPP_

#include <optional>
#include <rfl.hpp>
#include <type_traits>

#include "has_schema.hpp"

namespace sqlgen::parsing {

template <class T>
std::optional<std::string> get_schema() noexcept {
  using Type = std::remove_cvref_t<T>;
  if constexpr (has_schema<Type>) {
    using LiteralType = typename Type::schema;
    return LiteralType().str();
  } else {
    return std::nullopt;
  }
}

}  // namespace sqlgen::parsing

#endif
