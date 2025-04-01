#include "sqlgen/sqlite/Connection.hpp"

#include <sstream>

namespace sqlgen::sqlite {

Connection::~Connection() {
  if (conn_) {
    sqlite3_close(conn_);
  }
}

std::string Connection::create_table_to_sql(
    const dynamic::CreateTable& _stmt) noexcept {
  std::stringstream stream;
  stream << "CREATE TABLE ";
  if (_stmt.table.schema) {
    stream << "\"" << *_stmt.table.schema << "\".";
  }
  stream << "\"" << _stmt.table.name << "\" ";
  if (_stmt.if_not_exists) {
    stream << "IF NOT EXISTS ";
  }
  stream << "(";

  stream << ");";
  return stream.str();
}

rfl::Result<Ref<sqlgen::Connection>> Connection::make(
    const std::string& _fname) noexcept {
  try {
    return Ref<sqlgen::Connection>(Ref<Connection>::make(_fname));
  } catch (std::exception& e) {
    return error(e.what());
  }
}

Result<Nothing> Connection::exec(const std::string& _sql) noexcept {
  char* errmsg = nullptr;
  sqlite3_exec(conn_, _sql.c_str(), nullptr, nullptr, &errmsg);
  if (errmsg) {
    const auto err = error(errmsg);
    sqlite3_free(errmsg);
    return err;
  }
  return Nothing{};
}

sqlite3* Connection::make_conn(const std::string& _fname) {
  sqlite3* conn = nullptr;
  const auto err = sqlite3_open(_fname.c_str(), &conn);
  if (err) {
    throw std::runtime_error("Can't open database: " +
                             std::string(sqlite3_errmsg(conn)));
  }
  return conn;
}

Connection& Connection::operator=(Connection&& _other) {
  if (this == &_other) {
    return *this;
  }
  sqlite3_close(conn_);
  conn_ = _other.conn_;
  _other.conn_ = nullptr;
  return *this;
}

std::string Connection::to_sql(const dynamic::Statement& _stmt) noexcept {
  return _stmt.visit([&](const auto& _s) -> std::string {
    using S = std::remove_cvref_t<decltype(_s)>;
    if constexpr (std::is_same_v<S, dynamic::CreateTable>) {
      return create_table_to_sql(_s);
    } else {
      static_assert(rfl::always_false_v<S>, "Unsupported type.");
    }
  });
}

}  // namespace sqlgen::sqlite
