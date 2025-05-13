#ifndef SQLGEN_BEGIN_TRANSACTION_HPP_
#define SQLGEN_BEGIN_TRANSACTION_HPP_

#include <type_traits>

#include "Connection.hpp"
#include "Ref.hpp"
#include "Result.hpp"

namespace sqlgen {

inline Result<Ref<Connection>> begin_transaction_impl(
    const Ref<Connection>& _conn) {
  return _conn->begin_transaction().transform(
      [&](const auto&) { return _conn; });
}

inline Result<Ref<Connection>> begin_transaction_impl(
    const Result<Ref<Connection>>& _res) {
  return _res.and_then(
      [&](const auto& _conn) { return begin_transaction_impl(_conn); });
}

struct BeginTransaction {
  Result<Ref<Connection>> operator()(const auto& _conn) const noexcept {
    try {
      return begin_transaction_impl(_conn);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }
};

inline const auto begin_transaction = BeginTransaction{};

}  // namespace sqlgen

#endif
