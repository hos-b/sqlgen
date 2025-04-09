#ifndef SQLGEN_PARSING_TO_SELECT_FROM_HPP_
#define SQLGEN_PARSING_TO_SELECT_FROM_HPP_

#include <ranges>
#include <rfl.hpp>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "../dynamic/SelectFrom.hpp"
#include "../dynamic/Table.hpp"
#include "get_schema.hpp"
#include "get_tablename.hpp"
#include "make_columns.hpp"

namespace sqlgen::parsing {

template <class T>
  requires std::is_class_v<std::remove_cvref_t<T>> &&
           std::is_aggregate_v<std::remove_cvref_t<T>>
dynamic::SelectFrom to_select_from() {
  using NamedTupleType = rfl::named_tuple_t<std::remove_cvref_t<T>>;
  using Fields = typename NamedTupleType::Fields;

  const auto columns = make_columns<Fields>(
      std::make_integer_sequence<int, rfl::tuple_size_v<Fields>>());

  return dynamic::SelectFrom{.table = dynamic::Table{.name = get_tablename<T>(),
                                                     .schema = get_schema<T>()},
                             .columns = columns};
}

}  // namespace sqlgen::parsing

#endif
