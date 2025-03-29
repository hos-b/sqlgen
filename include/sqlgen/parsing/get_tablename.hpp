#ifndef SQLGEN_PARSING_GET_TABLENAME_HPP_
#define SQLGEN_PARSING_GET_TABLENAME_HPP_

#include <rfl.hpp>
#include <type_traits>

#include "has_tablename.hpp"

namespace rfl::parsing {

namespace internal {

std::string remove_namespaces(const std::string& _str) {
  const auto pos = _str.find_last_of(':');
  if (pos == std::string::npos) {
    return _str;
  }
  return _str.substr(pos + 1);
}

}  // namespace internal

template <class T>
std::string get_tablename() noexcept {
  using Type = std::remove_cvref_t<T>;
  if constexpr (has_tablename<Type>) {
    using LiteralType = typename Type::Tablename;
    return remove_namespaces(LiteralType().str());
  } else {
    return remove_namespaces(rfl::type_name_t<Type>().str());
  }
}

}  // namespace rfl::parsing

#endif
