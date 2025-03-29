#ifndef SQLGEN_PARSING_HAS_TABLENAME_HPP_
#define SQLGEN_PARSING_HAS_TABLENAME_HPP_

#include <concepts>

namespace sqlgen::parsing {

template <typename T>
concept has_tablename = requires() {
  { typename T::Tablename() } -> std::same_as<typename T::Tablename>;
};

}  // namespace sqlgen::parsing

#endif
