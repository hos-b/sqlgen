#ifndef SQLGEN_INTERNAL_FROM_STR_HPP_
#define SQLGEN_INTERNAL_FROM_STR_HPP_

#include <optional>
#include <string>
#include <type_traits>

#include "../Result.hpp"
#include "../parsing/has_reflection_method.hpp"
#include "../parsing/is_nullable.hpp"

namespace sqlgen::internal {

template <class T>
Result<T> from_str(const std::optional<std::string>& _str) {
  using Type = std::remove_cvref_t<T>;

  if constexpr (parsing::has_reflection_method<Type>) {
    return from_str<typename Type::ReflectionType>(_str).transform(
        [](auto&& _v) { return Type(std::move(_v)); });

  } else if constexpr (parsing::is_nullable_v<Type>) {
    if (!_str) {
      return Type();
    }

    if constexpr (parsing::is_ptr<Type>::value) {
      return from_str<typename Type::element_type>(_str).transform(
          [](auto&& _v) { return Type(std::move(_v)); });

    } else {
      return from_str<typename Type::value_type>(_str).transform(
          [](auto&& _v) { return Type(std::move(_v)); });
    }

  } else {
    if (!_str) {
      return error("Encountered NULL value.");
    }

    if constexpr (std::is_same_v<Type, std::string>) {
      return *_str;

    } else {
      try {
        if constexpr (std::is_floating_point_v<Type>) {
          return static_cast<Type>(std::stod(*_str));
        } else if constexpr (std::is_integral_v<Type>) {
          return static_cast<type>(std::stoll(*_str));
        } else if (std::is_same_v<Type, bool>) {
          return std::stoi(*_str) != 0;
        } else {
          static_assert(rfl::always_false<Type>, "Unsupported type");
        }

      } catch (std::exception& e) {
        return error(e.what());
      }
    }
  }
}

}  // namespace sqlgen::internal

#endif
