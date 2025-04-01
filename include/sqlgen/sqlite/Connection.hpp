#ifndef SQLGEN_SQLITE3_CONNECTION_HPP_
#define SQLGEN_SQLITE3_CONNECTION_HPP_

#include <sqlite3.h>

#include <memory>
#include <rfl.hpp>
#include <stdexcept>
#include <string>

#include "../Connection.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../dynamic/Statement.hpp"

namespace sqlgen::sqlite {

class Connection : public sqlgen::Connection {
 public:
  Connection(const std::string& _fname) : conn_(make_conn(_fname)) {}

  Connection(const Connection& _other) = delete;

  Connection(Connection&& _other) : conn_(_other.conn_) {
    _other.conn_ = nullptr;
  }

  static rfl::Result<Ref<sqlgen::Connection>> make(
      const std::string& _fname) noexcept;

  ~Connection();

  Result<Nothing> commit() final { return exec("COMMIT;"); }

  Result<Nothing> execute(const dynamic::Statement& _stmt) final {
    return exec(to_sql(_stmt));
  }

  Connection& operator=(const Connection& _other) = delete;

  Connection& operator=(Connection&& _other);

  Result<Ref<Iterator>> read(const dynamic::SelectFrom& _query) final {
    return error("TODO");
  }

  Result<Nothing> start_write(const dynamic::Insert& _stmt) final {
    return error("TODO");
  }

  Result<Nothing> end_write() final { return error("TODO"); }

  Result<Nothing> write(
      const std::vector<std::vector<std::optional<std::string>>>& _data) final {
    return error("TODO");
  }

 private:
  /// Transforms a CreateTable Statement to an SQL string.
  std::string create_table_to_sql(const dynamic::CreateTable& _stmt) noexcept;

  /// Wrapper around sqlite3_exec.
  Result<Nothing> exec(const std::string& _sql) noexcept;

  /// Transforms a Statement to an SQL string.
  std::string to_sql(const dynamic::Statement& _stmt) noexcept;

  /// Generates the underlying connection.
  static sqlite3* make_conn(const std::string& _fname);

 private:
  /// The underlying sqlite3 connection.
  sqlite3* conn_;
};

}  // namespace sqlgen::sqlite

#endif
