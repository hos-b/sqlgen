#include "sqlgen/postgres/Connection.hpp"

#include <ranges>
#include <rfl.hpp>
#include <sstream>
#include <stdexcept>

#include "sqlgen/internal/collect/vector.hpp"
#include "sqlgen/internal/strings/strings.hpp"

namespace sqlgen::postgres {

rfl::Result<Ref<sqlgen::Connection>> Connection::make(
    const Credentials& _credentials) noexcept {
  try {
    return Ref<sqlgen::Connection>(Ref<Connection>::make(_credentials));
  } catch (std::exception& e) {
    return error(e.what());
  }
}

typename Connection::ConnPtr Connection::make_conn(
    const std::string& _conn_str) {
  const auto raw_ptr = PQconnectdb(_conn_str.c_str());

  if (PQstatus(raw_ptr) != CONNECTION_OK) {
    const auto msg = std::string("Connection to postgres failed: ") +
                     PQerrorMessage(raw_ptr);
    PQfinish(raw_ptr);
    throw std::runtime_error(msg.c_str());
  }

  return ConnPtr::make(std::shared_ptr<PGconn>(raw_ptr, &PQfinish)).value();
}

std::string Connection::select_from_to_sql(
    const dynamic::SelectFrom& _stmt) noexcept {
  using namespace std::ranges::views;

  const auto to_str = [](const auto& _col) -> std::string {
    return "\"" + _col.name + "\"";
  };

  std::stringstream stream;
  stream << "SELECT ";
  stream << internal::strings::join(
      ", ", internal::collect::vector(_stmt.columns | transform(to_str)));

  stream << " FROM ";
  if (_stmt.table.schema) {
    stream << "\"" << *_stmt.table.schema << "\".";
  }
  stream << "\"" << _stmt.table.name << "\";";

  return stream.str();
}

std::string Connection::to_sql(const dynamic::Statement& _stmt) noexcept {
  return _stmt.visit([&](const auto& _s) -> std::string {
    using S = std::remove_cvref_t<decltype(_s)>;
    /*if constexpr (std::is_same_v<S, dynamic::CreateTable>) {
      return create_table_to_sql(_s);
    } else if constexpr (std::is_same_v<S, dynamic::Insert>) {
      return insert_to_sql(_s);
    } else*/
    if constexpr (std::is_same_v<S, dynamic::SelectFrom>) {
      return select_from_to_sql(_s);
    } else {
      return "TODO";
      // static_assert(rfl::always_false_v<S>, "Unsupported type.");
    }
  });
}

}  // namespace sqlgen::postgres
