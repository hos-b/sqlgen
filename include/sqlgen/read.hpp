#ifndef SQLGEN_READ_HPP_
#define SQLGEN_READ_HPP_

#include <type_traits>

#include "Connection.hpp"
#include "Range.hpp"
#include "Ref.hpp"
#include "Result.hpp"
#include "internal/is_range.hpp"
#include "transpilation/OrderBy.hpp"
#include "transpilation/to_select_from.hpp"

namespace sqlgen {

template <class ContainerType, class OrderByType>
Result<ContainerType> read_impl(const Ref<Connection>& _conn) {
  if constexpr (internal::is_range_v<ContainerType>) {
    using ValueType = typename ContainerType::value_type::value_type;
    const auto query = transpilation::to_select_from<ValueType, OrderByType>();
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
    return read_impl<Range<ValueType>, OrderByType>(_conn).and_then(
        to_container);
  }
}

template <class ContainerType, class OrderByType>
Result<ContainerType> read_impl(const Result<Ref<Connection>>& _res) {
  return _res.and_then([](const auto& _conn) {
    return read_impl<ContainerType, OrderByType>(_conn);
  });
}

template <class ContainerType, class OrderByType = Nothing>
struct Read {
  Result<ContainerType> operator()(const auto& _conn) const noexcept {
    try {
      return read_impl<ContainerType, OrderByType>(_conn);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  template <class... ColTypes>
  auto order_by(const ColTypes&...) const noexcept {
    static_assert(std::is_same_v<OrderByType, Nothing>,
                  "order_by is already assigned.");
    static_assert(sizeof...(ColTypes) != 0,
                  "You must assign at least one column to order by.");
    return Read<
        ContainerType,
        transpilation::order_by_t<typename ContainerType::value_type,  // TODO
                                  std::remove_cvref_t<ColTypes>...>>{};
  }

  OrderByType order_by_;
};

template <class ContainerType>
const auto read = Read<ContainerType>{};

}  // namespace sqlgen

#endif
