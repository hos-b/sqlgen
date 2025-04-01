#ifndef SQLGEN_DYNAMIC_COLUMN_HPP_
#define SQLGEN_DYNAMIC_COLUMN_HPP_

#include <optional>
#include <string>
#include <vector>

#include "Type.hpp"

namespace sqlgen::dynamic {

struct Column {
  std::optional<std::string> alias;
  std::string name;
  Type type;
};

}  // namespace sqlgen::dynamic

#endif
