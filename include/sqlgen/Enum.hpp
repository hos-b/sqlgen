#ifndef SQLGEN_ENUM_HPP_
#define SQLGEN_ENUM_HPP_

#include <rfl.hpp>

namespace sqlgen {

template <typename T>
concept IntEnum = std::is_enum_v<T> && requires(T t) {
  { static_cast<int>(t) } -> std::convertible_to<int>;
};

template <IntEnum T, bool _integer_rep = false>
struct Enum {
 private:
  static constexpr auto _get_enum_names() {
    constexpr auto enumerators = rfl::get_enumerator_array<T>();
    std::array<std::string_view, std::size(enumerators)> names{};
    for (std::size_t i = 0; i < std::size(enumerators); ++i) {
      names[i] = enumerators[i].first;
    }
    return names;
  }

 public:
  using UnderlyingType = T;
  static constexpr bool integer_rep = _integer_rep;
  static constexpr auto enum_names = _get_enum_names();

  std::string str() const { return rfl::enum_to_string(value); }

  Enum& operator=(const UnderlyingType& val) {
    value = val;
    return *this;
  }

  operator UnderlyingType() const { return value; }

  UnderlyingType value;
};

namespace parsing {
template <IntEnum T>
struct Parser<sqlgen::Enum<T>> {
  using Type = sqlgen::Enum<T>;

  static Result<Enum<T>> read(const std::optional<std::string>& _str) noexcept {
    if (!_str) {
      return error("NULL value encountered: Enum cannot be NULL.");
    }
    if constexpr (Enum<T>::integer_rep) {
      try {
        return Enum<T>{
            static_cast<typename Enum<T>::UnderlyingType>(std::stoi(*_str))};
      } catch (const std::exception& e) {
        return error(e.what());
      }
    }
    auto res = rfl::string_to_enum<typename Enum<T>::UnderlyingType>(*_str);
    if (res)
      return Enum<T>{*res};
    else
      return error(res.error());
  }

  static std::optional<std::string> write(const Enum<T>& _enum) noexcept {
    if constexpr (Enum<T>::integer_rep) {
      return std::to_string(static_cast<int>(_enum.value));
    }
    return _enum.str();
  }

  static dynamic::Type to_type() noexcept {
    constexpr std::string_view tpname =
        rfl::internal::get_type_name_str_view<T>();
    return sqlgen::dynamic::types::Enum{
        std::string(tpname),
        std::vector<std::string>(sqlgen::Enum<T>::enum_names.begin(),
                                 sqlgen::Enum<T>::enum_names.end())};
  }
};
}  // namespace parsing
}  // namespace sqlgen

#endif