#ifndef SQLGEN_DYNAMIC_SELECTFROM_HPP_
#define SQLGEN_DYNAMIC_SELECTFROM_HPP_

#include <optional>
#include <string>
#include <vector>

#include "Column.hpp"
#include "Condition.hpp"
#include "Limit.hpp"
#include "OrderBy.hpp"
#include "Table.hpp"

namespace sqlgen::dynamic {

struct SelectFrom {
  Table table;
  std::vector<Column> columns;
  std::optional<Condition> where = std::nullopt;
  std::optional<OrderBy> order_by = std::nullopt;
  std::optional<Limit> limit = std::nullopt;
};

}  // namespace sqlgen::dynamic

#endif
