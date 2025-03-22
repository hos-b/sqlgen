#ifndef SQLGEN_PARSING_IS_NULLABLE_HPP_
#define SQLGEN_PARSING_IS_NULLABLE_HPP_

#include <memory>
#include <optional>
#include <type_traits>

namespace sqlgen::parsing {

/// Determines whether a field in a named tuple is required.
/// General case - most fields are required.
template <class T>
class has_nullopt;

template <class T>
class has_nullopt : public std::false_type {};

template <class T>
class has_nullopt<std::optional<T>> : public std::true_type {};

template <class T>
class has_nullopt<std::shared_ptr<T>> : public std::true_type {};

template <class T>
class has_nullopt<std::unique_ptr<T>> : public std::true_type {};

template <class T>
consteval bool is_nullable() {
  if constexpr (has_reflection_method<T>) {
    return is_nullable<typename T::ReflectionType>();
  } else {
    return has_nullopt<T>::value;
  }
}

template <class T>
constexpr bool is_nullable_v = is_nullable<std::remove_cvref_t<T>>();

}  // namespace sqlgen::parsing

#endif
