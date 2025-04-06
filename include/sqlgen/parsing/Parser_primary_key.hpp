#ifndef SQLGEN_PARSING_PARSER_PRIMARY_KEY_HPP_
#define SQLGEN_PARSING_PARSER_PRIMARY_KEY_HPP_

#include <string>
#include <type_traits>

#include "../PrimaryKey.hpp"
#include "../Result.hpp"
#include "../dynamic/Type.hpp"
#include "Parser_base.hpp"

namespace sqlgen::parsing {

template <class T>
struct Parser<PrimaryKey<T>> {
  static Result<PrimaryKey<T>> read(
      const std::optional<std::string>& _str) noexcept {
    return Parser<std::remove_cvref_t<T>>::read(_str).transform(
        [](auto&& _t) -> PrimaryKey<T> {
          return PrimaryKey<T>(std::move(_t));
        });
  }

  static std::optional<std::string> write(const PrimaryKey<T>& _p) noexcept {
    return Parser<std::remove_cvref_t<T>>::write(_p.value());
  }

  static dynamic::Type to_type() noexcept {
    return Parser<std::remove_cvref_t<T>>::to_type().visit(
        [](auto _t) -> dynamic::Type {
          _t.properties.primary = true;
          return _t;
        });
  }
};

}  // namespace sqlgen::parsing

#endif
