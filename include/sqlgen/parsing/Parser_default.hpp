#ifndef SQLGEN_PARSING_PARSER_DEFAULT_HPP_
#define SQLGEN_PARSING_PARSER_DEFAULT_HPP_

#include <rfl.hpp>
#include <string>
#include <type_traits>

#include "../Result.hpp"
#include "../dynamic/Type.hpp"
#include "../dynamic/types.hpp"
#include "../transpilation/has_reflection_method.hpp"
#include "Parser_base.hpp"

namespace sqlgen::parsing {

template <class T>
struct Parser {
  using Type = std::remove_cvref_t<T>;

  static Result<T> read(const std::optional<std::string>& _str) noexcept {
    if constexpr (transpilation::has_reflection_method<Type>) {
      return Parser<std::remove_cvref_t<typename Type::ReflectionType>>::read(
                 _str)
          .transform([](auto&& _t) { return Type(std::move(_t)); });

    } else {
      if (!_str) {
        return error("NULL value encounted: Numeric value cannot be NULL.");
      }

      try {
        if constexpr (std::is_floating_point_v<Type>) {
          return static_cast<Type>(std::stod(*_str));
        } else if constexpr (std::is_integral_v<Type>) {
          return static_cast<Type>(std::stoll(*_str));
        } else if (std::is_same_v<Type, bool>) {
          return std::stoi(*_str) != 0;
        } else {
          static_assert(rfl::always_false_v<Type>, "Unsupported type");
        }

      } catch (std::exception& e) {
        return error(e.what());
      }
    }
  }

  static std::optional<std::string> write(const T& _t) noexcept {
    if constexpr (transpilation::has_reflection_method<Type>) {
      return Parser<std::remove_cvref_t<typename Type::ReflectionType>>::write(
          _t.reflection());
    } else {
      return std::to_string(_t);
    }
  }

  static dynamic::Type to_type() noexcept {
    if constexpr (transpilation::has_reflection_method<Type>) {
      return Parser<
          std::remove_cvref_t<typename Type::ReflectionType>>::to_type();

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

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }
};

}  // namespace sqlgen::parsing

#endif
