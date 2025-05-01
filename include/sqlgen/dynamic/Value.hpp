#ifndef SQLGEN_DYNAMIC_VALUE_HPP_
#define SQLGEN_DYNAMIC_VALUE_HPP_

#include <rfl.hpp>
#include <string>

namespace sqlgen::dynamic {

struct Float {
  double val;
};

struct Integer {
  int64_t val;
};

struct String {
  std::string val;
};

using Value = rfl::TaggedUnion<"type", Float, Integer, String>;

}  // namespace sqlgen::dynamic

#endif
