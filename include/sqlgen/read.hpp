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
Result<ContainerType> read(const Ref<Connection>& _conn) noexcept {
  using ValueType = typename ContainerType::value_type;

  if constexpr (internal::is_range_v<ContainerType>) {
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

    return read<Range<ValueType>>(_conn).and_then(to_container);
  }
}

template <class ContainerType>
Result<ContainerType> read(const Result<Ref<Connection>>& _res) noexcept {
  return _res.and_then(
      [](const auto& _conn) { return read<ContainerType>(_conn); });
}

}  // namespace sqlgen

#endif
