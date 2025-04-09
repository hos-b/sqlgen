#ifndef SQLGEN_READ_HPP_
#define SQLGEN_READ_HPP_

#include <iterator>
#include <optional>
#include <rfl.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "Connection.hpp"
#include "Ref.hpp"
#include "Result.hpp"
#include "internal/to_str_vec.hpp"
#include "transpilation/to_create_table.hpp"
#include "transpilation/to_insert.hpp"

namespace sqlgen {

template <class ContainerType>
Result<ContainerType> read(const Ref<Connection>& _conn) {
  using T = std::remove_cvref_t<typename ContainerType::value_type>;

  const auto start_write = [&](const auto&) -> Result<Nothing> {
    const auto insert_stmt = transpilation::to_insert<T>();
    return _conn->start_write(insert_stmt);
  };

  const auto write = [&](const auto&) -> Result<Nothing> {
    try {
      std::vector<std::vector<std::optional<std::string>>> data;
      for (auto it = _begin; it != _end; ++it) {
        data.emplace_back(internal::to_str_vec(*it));
        if (data.size() == 50000) {
          _conn->write(data).value();
          data.clear();
        }
      }
      if (data.size() != 0) {
        _conn->write(data).value();
      }
      return Nothing{};
    } catch (std::exception& e) {
      _conn->end_write();
      return error(e.what());
    }
  };

  const auto end_write = [&](const auto&) -> Result<Nothing> {
    return _conn->end_write();
  };

  const auto create_table_stmt = transpilation::to_create_table<T>();

  return _conn->execute(_conn->to_sql(create_table_stmt))
      .and_then(start_write)
      .and_then(write)
      .and_then(end_write);
}

}  // namespace sqlgen

#endif
