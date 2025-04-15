#ifndef SQLGEN_READ_HPP_
#define SQLGEN_READ_HPP_

#include "Connection.hpp"
#include "Range.hpp"
#include "Ref.hpp"
#include "Result.hpp"
#include "internal/is_range.hpp"
#include "transpilation/to_select_from.hpp"

namespace sqlgen {

template <class ContainerType>
Result<ContainerType> read_impl(const Ref<Connection>& _conn) {
  if constexpr (internal::is_range_v<ContainerType>) {
    using ValueType = typename ContainerType::value_type::value_type;
    const auto query = transpilation::to_select_from<ValueType>();
    return _conn->read(query).transform(
        [](auto&& _it) { return ContainerType(_it); });

  } else {
    const auto to_container = [](auto range) -> Result<ContainerType> {
      ContainerType container;
      for (auto& res : range) {
        if (res) {
          container.emplace_back(std::move(*res));
        } else {
          return error(res.error().what());
        }
      }
      return container;
    };

    using ValueType = typename ContainerType::value_type;
    return read_impl<Range<ValueType>>(_conn).and_then(to_container);
  }
}

template <class ContainerType>
Result<ContainerType> read_impl(const Result<Ref<Connection>>& _res) {
  return _res.and_then(
      [](const auto& _conn) { return read_impl<ContainerType>(_conn); });
}

template <class ContainerType>
struct Read {
  Result<ContainerType> operator()(const auto& _conn) const noexcept {
    try {
      return read_impl<ContainerType>(_conn);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }
};

template <class ContainerType>
const auto read = Read<ContainerType>();

}  // namespace sqlgen

#endif
