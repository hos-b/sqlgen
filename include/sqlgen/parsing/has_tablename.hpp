#ifndef SQLGEN_PARSING_HAS_TABLENAME_HPP_
#define SQLGEN_PARSING_HAS_TABLENAME_HPP_

#include <concepts>
#include <string>

namespace sqlgen::parsing {

template <typename T>
concept has_tablename = requires() {
  { T::tablename } -> std::convertible_to<std::string>;
};

}  // namespace sqlgen::parsing

#endif
