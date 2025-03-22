#ifndef SQLGEN_PARSING_TO_CREATE_TABLE_HPP_
#define SQLGEN_PARSING_TO_CREATE_TABLE_HPP_

#include <rfl.hpp>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "../dynamic/Column.hpp"
#include "../dynamic/CreateTable.hpp"
#include "../dynamic/Table.hpp"
#include "../dynamic/Type.hpp"

namespace sqlgen::parsing {

namespace internal {

template <class FieldType>
std::string to_name() {
  using Name = typename FieldType::Name;
  return Name().str();
}

template <class FieldType>
dynamic::Type to_type() {
  // TODO
}

template <class FieldType>
dynamic::Column to_column() {
  return dynamic::Column{.name = to_name<FieldType>(),
                         .type = to_type<FieldType>()};
}

template <class Fields, int... _is>
std::vector<dynamic::Column> make_columns(std::integer_sequence<int, _is...>) {
  return std::vector<dynamic::Column>(
      {to_column<rfl::tuple_element_t<_is, Fields>>()...});
}

}  // namespace internal

template <class T>
  requires std::is_class_v<std::remove_cvref_t<T>> &&
           std::is_aggregate_v<std::remove_cvref_t<T>>
dynamic::CreateTable to_create_table() {
  using NamedTupleType = rfl::name_tuple_t<std::remove_cvref_t<T>>;
  using Fields = typename NamedTupleType::Fields;
  return dynamic::CreateTable{
      .table = dynamic::Table{.name = /*TODO*/},
      .columns = internal::make_columns<Fields>(
          std::make_integer_sequence<int, rfl::tuple_size_v<Fields>>),
      .if_not_exists = true};
}

}  // namespace sqlgen::parsing

#endif
