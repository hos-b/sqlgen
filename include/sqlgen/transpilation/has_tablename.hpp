#ifndef SQLGEN_TRANSPILATION_HAS_TABLENAME_HPP_
#define SQLGEN_TRANSPILATION_HAS_TABLENAME_HPP_

#include <concepts>
#include <string>

namespace sqlgen::transpilation {

template <typename T>
concept has_tablename = requires() {
  { T::tablename } -> std::convertible_to<std::string>;
};

}  // namespace sqlgen::transpilation

#endif
