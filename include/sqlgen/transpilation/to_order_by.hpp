#ifndef SQLGEN_TRANSPILATION_TO_ORDER_BY_HPP_
#define SQLGEN_TRANSPILATION_TO_ORDER_BY_HPP_

#include <vector>

#include "../Result.hpp"
#include "../dynamic/OrderBy.hpp"
#include "OrderBy.hpp"

namespace sqlgen::transpilation {

template <class T>
struct ToOrderBy;

template <>
struct ToOrderBy<Nothing> {
  std::optional<dynamic::OrderBy> operator()() const { return std::nullopt; }
};

template <class... Wrappers>
struct ToOrderBy<OrderBy<Wrappers...>> {
  template <class W>
  dynamic::Wrapper to_wrapper() const {
    using Name = typename W::ColType::Name;
    const auto column = dynamic::Column{.alias = std::nullopt,
                                        .name = Name().str(),
                                        .type = dynamic::types::Unknown{}};
    return dynamic::Wrapper{.column = column, .desc = W::desc};
  }

  std::optional<dynamic::OrderBy> operator()() const {
    const auto columns =
        std::vector<dynamic::Wrapper>({to_wrapper<Wrappers>()...});
    return dynamic::OrderBy{.columns = columns};
  }
};

template <class T>
const auto to_order_by = ToOrderBy<T>{};

}  // namespace sqlgen::transpilation

#endif
