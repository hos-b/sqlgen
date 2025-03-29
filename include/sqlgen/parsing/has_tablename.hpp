#ifndef SQLGEN_PARSING_HAS_TABLENAME_HPP_
#define SQLGEN_PARSING_HAS_TABLENAME_HPP_

#include <concepts>

namespace sqlgen::parsing {

template <typename T>
concept has_tablename = requires() {
  { typename T::tablename() } -> std::same_as<typename T::tablename>;
};

}  // namespace sqlgen::parsing

#endif
