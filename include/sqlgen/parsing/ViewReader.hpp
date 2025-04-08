#ifndef SQLGEN_PARSING_VIEWREADER_HPP_
#define SQLGEN_PARSING_VIEWREADER_HPP_

#include <array>
#include <optional>
#include <rfl.hpp>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "../Result.hpp"
#include "Parser.hpp"

namespace sqlgen::parsing {

template <class ViewType>
class ViewReader {
 private:
  static constexpr size_t size_ = ViewType::size();

 public:
  ViewReader(ViewType* _view) : view_(_view) {}

  ~ViewReader() = default;

  std::pair<std::optional<Error>, size_t> read(
      const std::vector<std::optional<std::string>>& _row) noexcept {
    if (_row.size() != size_) {
      std::stringstream stream;
      stream << "Expected exactly " << std::to_string(size_)
             << " fields, but got " << _row.size() << ".";
      return std::make_pair(Error(stream.str()), 0);
    }
    for (size_t i = 0; i < size_; ++i) {
      const auto err = assign_to_field_i(
          _row, i, &view_, std::make_integer_sequence<size_t, size_>());
      if (err) {
        return std::make_pair(err, i);
      }
    }
    return std::make_pair(std::nullopt, size_);
  }

 private:
  template <size_t i>
  static void assign_if_field_is_field_i(
      const std::vector<std::optional<std::string>>& _row, const size_t _i,
      ViewType* _view, std::optional<Error>* _err) noexcept {
    using FieldType = rfl::tuple_element_t<i, typename ViewType::Fields>;
    using OriginalType = typename FieldType::Type;
    using T =
        std::remove_cvref_t<std::remove_pointer_t<typename FieldType::Type>>;
    constexpr auto name = FieldType::name();
    if (_i == i) {
      auto res = Parser<T>::read(_row[i]);
      if (!res) {
        std::stringstream stream;
        stream << "Failed to parse field '" << std::string(name)
               << "': " << res.error().what();
        *_err = Error(stream.str());
        return;
      }
      rfl::get<i>(*_view) = std::move(*res);
    }
  }

  template <size_t... is>
  static std::optional<Error> assign_to_field_i(
      const std::vector<std::optional<std::string>>& _row, const size_t _i,
      ViewType* _view, std::integer_sequence<size_t, is...>) noexcept {
    std::optional<Error> err;
    (assign_if_field_is_field_i<is>(_row, _i, _view, &err), ...);
    return err;
  }

 private:
  ViewType* view_;
};

}  // namespace sqlgen::parsing

#endif
