#ifndef SQLGEN_PARSING_TO_INSERT_HPP_
#define SQLGEN_PARSING_TO_INSERT_HPP_

#include <ranges>
#include <rfl.hpp>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "../dynamic/Insert.hpp"
#include "../dynamic/Table.hpp"
#include "../internal/collect/vector.hpp"
#include "get_schema.hpp"
#include "get_tablename.hpp"
#include "make_columns.hpp"

namespace sqlgen::parsing {

template <class T>
  requires std::is_class_v<std::remove_cvref_t<T>> &&
           std::is_aggregate_v<std::remove_cvref_t<T>>
dynamic::Insert to_insert() {
  using namespace std::ranges::views;

  using NamedTupleType = rfl::named_tuple_t<std::remove_cvref_t<T>>;
  using Fields = typename NamedTupleType::Fields;

  const auto columns = make_columns<Fields>(
      std::make_integer_sequence<int, rfl::tuple_size_v<Fields>>());

  const auto get_name = [](const auto& _col) { return _col.name; };

  return dynamic::Insert{.table = dynamic::Table{.name = get_tablename<T>(),
                                                 .schema = get_schema<T>()},
                         .columns = sqlgen::internal::collect::vector(
                             columns | transform(get_name))};
}

}  // namespace sqlgen::parsing

#endif
