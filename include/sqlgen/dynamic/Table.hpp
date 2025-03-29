#ifndef SQLGEN_DYNAMIC_TABLE_HPP_
#define SQLGEN_DYNAMIC_TABLE_HPP_

#include <string>
#include <vector>

#include "Column.hpp"

namespace sqlgen::dynamic {

struct Table {
  std::string alias;
  std::string name;
  std::string schema;
};

}  // namespace sqlgen::dynamic

#endif
