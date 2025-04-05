#ifndef SQLGEN_PARSING_MAKE_COLUMNS_HPP_
#define SQLGEN_PARSING_MAKE_COLUMNS_HPP_

#include <rfl.hpp>
#include <string>
#include <type_traits>

#include "../dynamic/Column.hpp"
#include "../dynamic/Type.hpp"
#include "../dynamic/types.hpp"
#include "has_reflection_method.hpp"
#include "is_nullable.hpp"
#include "is_primary_key.hpp"

namespace sqlgen::parsing {

template <class Name>
std::string to_colname() {
  return Name().str();
}

template <class Type>
dynamic::Type to_type() {
  using T = std::remove_cvref_t<Type>;
  if constexpr (is_primary_key_v<T>) {
    return to_type<typename T::ReflectionType>().visit(
        [](auto _t) -> dynamic::Type {
          _t.properties.primary = true;
          return _t;
        });

  } else if constexpr (is_nullable_v<T>) {
    const auto set_nullable = [](auto _t) -> dynamic::Type {
      _t.properties.nullable = true;
      return _t;
    };
    if constexpr (is_ptr<T>::value) {
      return to_type<typename T::element_type>().visit(set_nullable);
    } else {
      return to_type<typename T::value_type>().visit(set_nullable);
    }

  } else if constexpr (has_reflection_method<T>) {
    return to_type<typename Type::ReflectionType>();

  } else if constexpr (std::is_same_v<T, bool>) {
    return dynamic::types::Boolean{};

  } else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
    if constexpr (sizeof(T) == 1) {
      return dynamic::types::Int8{};

    } else if constexpr (sizeof(T) == 2) {
      return dynamic::types::Int16{};

    } else if constexpr (sizeof(T) == 4) {
      return dynamic::types::Int32{};

    } else if constexpr (sizeof(T) == 8) {
      return dynamic::types::Int64{};

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported signed integer.");
    }
  } else if constexpr (std::is_integral_v<T> && !std::is_signed_v<T>) {
    if constexpr (sizeof(T) == 1) {
      return dynamic::types::UInt8{};

    } else if constexpr (sizeof(T) == 2) {
      return dynamic::types::UInt16{};

    } else if constexpr (sizeof(T) == 4) {
      return dynamic::types::UInt32{};

    } else if constexpr (sizeof(T) == 8) {
      return dynamic::types::UInt64{};

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported unsigned integer.");
    }
  } else if constexpr (std::is_floating_point_v<T>) {
    if constexpr (sizeof(T) == 4) {
      return dynamic::types::Float32{};

    } else if constexpr (sizeof(T) == 8) {
      return dynamic::types::Float64{};

    } else {
      static_assert(rfl::always_false_v<T>,
                    "Unsupported floating point value.");
    }
  } else if constexpr (std::is_same_v<T, std::string>) {
    return dynamic::types::Text{};
  }
}

template <class FieldType>
dynamic::Column to_column() {
  return dynamic::Column{.name = to_colname<typename FieldType::Name>(),
                         .type = to_type<typename FieldType::Type>()};
}

template <class Fields, int... _is>
std::vector<dynamic::Column> make_columns(std::integer_sequence<int, _is...>) {
  return std::vector<dynamic::Column>(
      {to_column<rfl::tuple_element_t<_is, Fields>>()...});
}

}  // namespace sqlgen::parsing

#endif
