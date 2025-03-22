#ifndef SQLGEN_PARSING_HAS_REFLECTION_TYPE_HPP_
#define SQLGEN_PARSING_HAS_REFLECTION_TYPE_HPP_

#include <concepts>

namespace sqlgen::parsing {

template <typename T>
concept has_reflection_method = requires(T _t) {
  { _t.reflection() } -> std::convertible_to<typename T::ReflectionType>;
};

}  // namespace sqlgen::parsing

#endif
