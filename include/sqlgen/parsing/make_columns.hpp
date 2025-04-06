#ifndef SQLGEN_PARSING_MAKE_COLUMNS_HPP_
#define SQLGEN_PARSING_MAKE_COLUMNS_HPP_

#include <rfl.hpp>
#include <string>
#include <type_traits>

#include "../dynamic/Column.hpp"
#include "../dynamic/Type.hpp"
#include "../dynamic/types.hpp"
#include "Parser.hpp"
#include "has_reflection_method.hpp"
#include "is_nullable.hpp"
#include "is_primary_key.hpp"

namespace sqlgen::parsing {

template <class Name>
std::string to_colname() {
  return Name().str();
}

template <class Type>
dynamic::Type to_type() {
  using T = std::remove_cvref_t<Type>;
  return Parser<T>::to_type();
}

template <class FieldType>
dynamic::Column to_column() {
  return dynamic::Column{.name = to_colname<typename FieldType::Name>(),
                         .type = to_type<typename FieldType::Type>()};
}

template <class Fields, int... _is>
std::vector<dynamic::Column> make_columns(std::integer_sequence<int, _is...>) {
  return std::vector<dynamic::Column>(
      {to_column<rfl::tuple_element_t<_is, Fields>>()...});
}

}  // namespace sqlgen::parsing

#endif
