#include "sqlgen/sqlite/Connection.hpp"

#include <ranges>
#include <rfl.hpp>
#include <sstream>

#include "sqlgen/internal/collect/vector.hpp"
#include "sqlgen/internal/strings/strings.hpp"
#include "sqlgen/sqlite/Iterator.hpp"
#include "sqlgen/sqlite/to_sql.hpp"

namespace sqlgen::sqlite {

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

typename Connection::ConnPtr Connection::make_conn(const std::string& _fname) {
  sqlite3* conn = nullptr;
  const auto err = sqlite3_open(_fname.c_str(), &conn);
  if (err) {
    throw std::runtime_error("Can't open database: " +
                             std::string(sqlite3_errmsg(conn)));
  }
  return ConnPtr::make(std::shared_ptr<sqlite3>(conn, &sqlite3_close)).value();
}

Result<Ref<IteratorBase>> Connection::read(const dynamic::SelectFrom& _query) {
  const auto sql = to_sql_impl(_query);

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

  return Ref<sqlite3_stmt>::make(StmtPtr(p_stmt, &sqlite3_finalize))
      .transform([&](auto _stmt) -> Ref<IteratorBase> {
        return Ref<Iterator>::make(_stmt, conn_);
      });
}

Result<Nothing> Connection::start_write(const dynamic::Insert& _stmt) {
  if (stmt_) {
    return error(
        "A write operation has already been launched. You need to call "
        ".end_write() before you can start another.");
  }

  const auto sql = to_sql_impl(_stmt);

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

}  // namespace sqlgen::sqlite
