#include "sqlgen/sqlite/Connection.hpp"

#include <ranges>
#include <rfl.hpp>
#include <sstream>

#include "sqlgen/internal/collect/vector.hpp"
#include "sqlgen/internal/strings/strings.hpp"

namespace sqlgen::sqlite {

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

  if (_stmt.if_not_exists) {
    stream << "IF NOT EXISTS ";
  }

  if (_stmt.table.schema) {
    stream << "\"" << *_stmt.table.schema << "\".";
  }
  stream << "\"" << _stmt.table.name << "\" ";

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
  sqlite3_exec(conn_.get(), _sql.c_str(), nullptr, nullptr, &errmsg);
  if (errmsg) {
    const auto err = error(errmsg);
    sqlite3_free(errmsg);
    return err;
  }
  return Nothing{};
}

std::string Connection::insert_to_sql(const dynamic::Insert& _stmt) noexcept {
  using namespace std::ranges::views;

  const auto in_quotes = [](const std::string& _str) -> std::string {
    return "\"" + _str + "\"";
  };

  const auto to_questionmark = [](const std::string&) -> std::string {
    return "?";
  };

  std::stringstream stream;
  stream << "INSERT INTO ";
  if (_stmt.table.schema) {
    stream << "\"" << *_stmt.table.schema << "\".";
  }
  stream << "\"" << _stmt.table.name << "\"";

  stream << " (";
  stream << internal::strings::join(
      ", ", internal::collect::vector(_stmt.columns | transform(in_quotes)));
  stream << ")";

  stream << " VALUES (";
  stream << internal::strings::join(
      ", ",
      internal::collect::vector(_stmt.columns | transform(to_questionmark)));
  stream << ");";

  return stream.str();
}

typename Connection::ConnPtr Connection::make_conn(const std::string& _fname) {
  sqlite3* conn = nullptr;
  const auto err = sqlite3_open(_fname.c_str(), &conn);
  if (err) {
    throw std::runtime_error("Can't open database: " +
                             std::string(sqlite3_errmsg(conn)));
  }
  return ConnPtr(conn, &sqlite3_close);
}

std::string Connection::properties_to_sql(
    const dynamic::types::Properties& _p) noexcept {
  return std::string(_p.primary ? " PRIMARY KEY" : "") +
         std::string(_p.nullable ? "" : " NOT NULL");
}

std::string Connection::to_sql(const dynamic::Statement& _stmt) noexcept {
  return _stmt.visit([&](const auto& _s) -> std::string {
    using S = std::remove_cvref_t<decltype(_s)>;
    if constexpr (std::is_same_v<S, dynamic::CreateTable>) {
      return create_table_to_sql(_s);
    } else if constexpr (std::is_same_v<S, dynamic::Insert>) {
      return insert_to_sql(_s);
    } else {
      static_assert(rfl::always_false_v<S>, "Unsupported type.");
    }
  });
}

Result<Nothing> Connection::start_write(const dynamic::Insert& _stmt) {
  if (stmt_) {
    return error(
        "A write operation has already been launched. You need to call "
        ".end_write() before you can start another.");
  }

  const auto sql = to_sql(_stmt);

  sqlite3_stmt* p_stmt = nullptr;

  sqlite3_prepare(conn_.get(), /* Database handle */
                  sql.c_str(), /* SQL statement, UTF-8 encoded */
                  sql.size(),  /* Maximum length of zSql in bytes. */
                  &p_stmt,     /* OUT: Statement handle */
                  nullptr      /* OUT: Pointer to unused portion of zSql */
  );

  if (!p_stmt) {
    return error(sqlite3_errmsg(conn_.get()));
  }

  stmt_ = StmtPtr(p_stmt, &sqlite3_finalize);

  return Nothing{};
}

Result<Nothing> Connection::write(
    const std::vector<std::vector<std::optional<std::string>>>& _data) {
  if (!stmt_) {
    return error(
        " You need to call .start_write(...) before you can call "
        ".write(...).");
  }

  for (const auto& row : _data) {
    const auto num_cols = static_cast<int>(row.size());

    for (int i = 0; i < num_cols; ++i) {
      if (row[i]) {
        const auto res =
            sqlite3_bind_text(stmt_.get(), i + 1, row[i]->c_str(),
                              static_cast<int>(row[i]->size()), SQLITE_STATIC);
        if (res != SQLITE_OK) {
          return error(sqlite3_errmsg(conn_.get()));
        }
      } else {
        const auto res = sqlite3_bind_null(stmt_.get(), i + 1);
        if (res != SQLITE_OK) {
          return error(sqlite3_errmsg(conn_.get()));
        }
      }
    }

    auto res = sqlite3_step(stmt_.get());
    if (res != SQLITE_OK && res != SQLITE_ROW && res != SQLITE_DONE) {
      return error(sqlite3_errmsg(conn_.get()));
    }

    res = sqlite3_reset(stmt_.get());
    if (res != SQLITE_OK) {
      return error(sqlite3_errmsg(conn_.get()));
    }
  }

  // We need to reset the statement to avoid segfaults.
  const auto res = sqlite3_clear_bindings(stmt_.get());
  if (res != SQLITE_OK) {
    return error(sqlite3_errmsg(conn_.get()));
  }

  return Nothing{};
}

Result<Nothing> Connection::end_write() {
  if (!stmt_) {
    return error(
        " You need to call .start_write(...) before you can call "
        ".end_write().");
  }
  stmt_ = nullptr;
  return Nothing{};
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
