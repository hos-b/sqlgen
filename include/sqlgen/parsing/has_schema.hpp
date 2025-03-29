#ifndef SQLGEN_PARSING_HAS_SCHEMA_HPP_
#define SQLGEN_PARSING_HAS_SCHEMA_HPP_

#include <concepts>

namespace sqlgen::parsing {

template <typename T>
concept has_schema = requires() {
  { typename T::schema() } -> std::same_as<typename T::schema>;
};

}  // namespace sqlgen::parsing

#endif
