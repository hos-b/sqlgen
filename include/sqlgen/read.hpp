#ifndef SQLGEN_READ_HPP_
#define SQLGEN_READ_HPP_

#include <type_traits>

#include "Connection.hpp"
#include "Range.hpp"
#include "Ref.hpp"
#include "Result.hpp"
#include "internal/is_range.hpp"
#include "transpilation/to_select_from.hpp"
#include "transpilation/value_t.hpp"

namespace sqlgen {

template <class ContainerType, class WhereType, class OrderByType,
          class LimitType>
Result<ContainerType> read_impl(const Ref<Connection>& _conn,
                                const WhereType& _where,
                                const LimitType& _limit) {
  using ValueType = transpilation::value_t<ContainerType>;
  if constexpr (internal::is_range_v<ContainerType>) {
    const auto query =
        transpilation::to_select_from<ValueType, WhereType, OrderByType,
                                      LimitType>(_where, _limit);
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

    return read_impl<Range<ValueType>, WhereType, OrderByType, LimitType>(
               _conn, _where, _limit)
        .and_then(to_container);
  }
}

template <class ContainerType, class WhereType, class OrderByType,
          class LimitType>
Result<ContainerType> read_impl(const Result<Ref<Connection>>& _res,
                                const WhereType& _where,
                                const LimitType& _limit) {
  return _res.and_then([&](const auto& _conn) {
    return read_impl<ContainerType, WhereType, OrderByType, LimitType>(
        _conn, _where, _limit);
  });
}

template <class ContainerType, class WhereType = Nothing,
          class OrderByType = Nothing, class LimitType = Nothing>
struct Read {
  Result<ContainerType> operator()(const auto& _conn) const noexcept {
    try {
      return read_impl<ContainerType, WhereType, OrderByType, LimitType>(
          _conn, where_, limit_);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  WhereType where_;

  OrderByType order_by_;

  LimitType limit_;
};

template <class ContainerType>
const auto read = Read<ContainerType>{};

}  // namespace sqlgen

#endif
