#ifndef SQLGEN_DYNAMIC_SELECTFROM_HPP_
#define SQLGEN_DYNAMIC_SELECTFROM_HPP_

#include <string>
#include <vector>

#include "Column.hpp"
#include "Table.hpp"

namespace sqlgen::dynamic {

struct SelectFrom {
  Table table;
  std::vector<Column> columns;
};

}  // namespace sqlgen::dynamic

#endif
