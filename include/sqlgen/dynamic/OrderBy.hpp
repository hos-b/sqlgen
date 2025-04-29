#ifndef SQLGEN_DYNAMIC_ORDERBY_HPP_
#define SQLGEN_DYNAMIC_ORDERBY_HPP_

#include <vector>

#include "Column.hpp"

namespace sqlgen::dynamic {

struct OrderBy {
  struct Wrapper {
    Column column;
    bool desc = false;
  };
  std::vector<Wrapper> columns;
};

}  // namespace sqlgen::dynamic

#endif
