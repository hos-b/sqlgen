#ifndef SQLGEN_TRANSPILATION_TO_DELETE_FROM_HPP_
#define SQLGEN_TRANSPILATION_TO_DELETE_FROM_HPP_

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "../Result.hpp"
#include "../dynamic/DeleteFrom.hpp"
#include "../dynamic/Table.hpp"
#include "get_schema.hpp"
#include "get_tablename.hpp"
#include "to_condition.hpp"

namespace sqlgen::transpilation {

template <class T, class WhereType>
  requires std::is_class_v<std::remove_cvref_t<T>> &&
           std::is_aggregate_v<std::remove_cvref_t<T>>
dynamic::DeleteFrom to_delete_from(const WhereType& _where) {
  return dynamic::DeleteFrom{
      .table =
          dynamic::Table{.name = get_tablename<T>(), .schema = get_schema<T>()},
      .where = to_condition<std::remove_cvref_t<T>>(_where)};
}

}  // namespace sqlgen::transpilation

#endif
