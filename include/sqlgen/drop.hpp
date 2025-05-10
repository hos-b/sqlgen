#ifndef SQLGEN_DROP_HPP_
#define SQLGEN_DROP_HPP_

#include <type_traits>

#include "Connection.hpp"
#include "Ref.hpp"
#include "Result.hpp"
#include "transpilation/to_drop.hpp"

namespace sqlgen {

template <class ValueType>
Result<Nothing> drop_impl(const Ref<Connection>& _conn, const bool _if_exists) {
  const auto query = transpilation::to_drop<ValueType>(_if_exists);
  return _conn->execute(_conn->to_sql(query));
}

template <class ValueType>
Result<Nothing> drop_impl(const Result<Ref<Connection>>& _res,
                          const bool _if_exists) {
  return _res.and_then([&](const auto& _conn) {
    return drop_impl<ValueType>(_conn, _if_exists);
  });
}

template <class ValueType>
struct Drop {
  Result<Nothing> operator()(const auto& _conn) const noexcept {
    try {
      return drop_impl<ValueType>(_conn, if_exists_);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  bool if_exists_ = false;
};

template <class ContainerType>
const auto drop = Drop<ContainerType>{};

}  // namespace sqlgen

#endif
