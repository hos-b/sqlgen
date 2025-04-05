#ifndef SQLGEN_PARSING_HAS_SCHEMA_HPP_
#define SQLGEN_PARSING_HAS_SCHEMA_HPP_

#include <concepts>
#include <string>

namespace sqlgen::parsing {

template <typename T>
concept has_schema = requires() {
  { T::schema } -> std::convertible_to<std::string>;
};

}  // namespace sqlgen::parsing

#endif
