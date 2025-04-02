#include "sqlgen/sqlite/Connection.hpp"

#include <ranges>
#include <rfl.hpp>
#include <sstream>

#include "sqlgen/internal/collect/vector.hpp"
#include "sqlgen/internal/strings/strings.hpp"

namespace sqlgen::sqlite {

Connection::~Connection() {
  if (conn_) {
    sqlite3_close(conn_);
  }
}

std::string Connection::column_to_sql_definition(
    const dynamic::Column& _col) noexcept {
  return "\"" + _col.name + "\"" + " " + type_to_sql(_col.type) +
         properties_to_sql(
             _col.type.visit([](const auto& _t) { return _t.properties; }));
}

std::string Connection::create_table_to_sql(
    const dynamic::CreateTable& _stmt) noexcept {
  using namespace std::ranges::views;

  const auto col_to_sql = [&](const auto& _col) {
    return column_to_sql_definition(_col);
  };

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
  stream << internal::strings::join(
      ", ", internal::collect::vector(_stmt.columns | transform(col_to_sql)));
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

Result<Nothing> Connection::execute(const std::string& _sql) noexcept {
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

std::string Connection::properties_to_sql(
    const dynamic::types::Properties& _p) noexcept {
  return std::string(_p.primary ? " PRIMARY KEY" : "") +
         std::string(_p.nullable ? "" : " NOT NULL");
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

std::string Connection::type_to_sql(const dynamic::Type& _type) noexcept {
  return _type.visit([](const auto _t) -> std::string {
    using T = std::remove_cvref_t<decltype(_t)>;
    if constexpr (std::is_same_v<T, dynamic::types::Boolean> ||
                  std::is_same_v<T, dynamic::types::Int8> ||
                  std::is_same_v<T, dynamic::types::Int16> ||
                  std::is_same_v<T, dynamic::types::Int32> ||
                  std::is_same_v<T, dynamic::types::Int64> ||
                  std::is_same_v<T, dynamic::types::UInt8> ||
                  std::is_same_v<T, dynamic::types::UInt16> ||
                  std::is_same_v<T, dynamic::types::UInt32> ||
                  std::is_same_v<T, dynamic::types::UInt64>) {
      return "INTEGER";
    } else if constexpr (std::is_same_v<T, dynamic::types::Float32> ||
                         std::is_same_v<T, dynamic::types::Float64>) {
      return "REAL";
    } else if constexpr (std::is_same_v<T, dynamic::types::Unknown> ||
                         std::is_same_v<T, dynamic::types::Text> ||
                         std::is_same_v<T, dynamic::types::VarChar> ||
                         std::is_same_v<T, dynamic::types::Timestamp> ||
                         std::is_same_v<T, dynamic::types::TimestampWithTZ>) {
      return "TEXT";
    } else {
      static_assert(rfl::always_false_v<T>, "Not all cases were covered.");
    }
  });
}

}  // namespace sqlgen::sqlite
