#ifndef SQLGEN_COMMIT_HPP_
#define SQLGEN_COMMIT_HPP_

#include <type_traits>

#include "Connection.hpp"
#include "Ref.hpp"
#include "Result.hpp"

namespace sqlgen {

inline Result<Ref<Connection>> commit_impl(const Ref<Connection>& _conn) {
  return _conn->commit().transform([&](const auto&) { return _conn; });
}

inline Result<Ref<Connection>> commit_impl(
    const Result<Ref<Connection>>& _res) {
  return _res.and_then([&](const auto& _conn) { return commit_impl(_conn); });
}

struct Commit {
  Result<Ref<Connection>> operator()(const auto& _conn) const noexcept {
    try {
      return commit_impl(_conn);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }
};

inline const auto commit = Commit{};

}  // namespace sqlgen

#endif
