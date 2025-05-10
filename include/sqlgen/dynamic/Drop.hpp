#ifndef SQLGEN_DYNAMIC_DROP_HPP_
#define SQLGEN_DYNAMIC_DROP_HPP_

#include <optional>

#include "Table.hpp"

namespace sqlgen::dynamic {

struct Drop {
  bool if_exists = true;
  Table table;
};

}  // namespace sqlgen::dynamic

#endif
