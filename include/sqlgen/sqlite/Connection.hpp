#ifndef SQLGEN_SQLITE_CONNECTION_HPP_
#define SQLGEN_SQLITE_CONNECTION_HPP_

#include <sqlite3.h>

#include <memory>
#include <rfl.hpp>
#include <stdexcept>
#include <string>

#include "../Connection.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../dynamic/Column.hpp"
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

  Result<Nothing> commit() final { return execute("COMMIT;"); }

  Result<Nothing> execute(const std::string& _sql) noexcept final;

  Connection& operator=(const Connection& _other) = delete;

  Connection& operator=(Connection&& _other);

  Result<Ref<Iterator>> read(const dynamic::SelectFrom& _query) final {
    return error("TODO");
  }

  std::string to_sql(const dynamic::Statement& _stmt) noexcept final;

  Result<Nothing> start_write(const dynamic::Insert& _stmt) final {
    return error("TODO");
  }

  Result<Nothing> end_write() final { return error("TODO"); }

  Result<Nothing> write(
      const std::vector<std::vector<std::optional<std::string>>>& _data) final {
    return error("TODO");
  }

 private:
  /// Transforms a dynamic::Column to an SQL string that defines the column in a
  /// CREATE TABLE statement.
  std::string column_to_sql_definition(const dynamic::Column& _col) noexcept;

  /// Transforms a CreateTable Statement to an SQL string.
  std::string create_table_to_sql(const dynamic::CreateTable& _stmt) noexcept;

  /// Transforms an Insert Statement to an SQL string.
  std::string insert_to_sql(const dynamic::Insert& _stmt) noexcept;

  /// Generates the underlying connection.
  static sqlite3* make_conn(const std::string& _fname);

  /// Expresses the properies as SQL.
  std::string properties_to_sql(const dynamic::types::Properties& _p) noexcept;

  /// Expresses the type as SQL.
  std::string type_to_sql(const dynamic::Type& _type) noexcept;

 private:
  /// The underlying sqlite3 connection.
  sqlite3* conn_;
};

}  // namespace sqlgen::sqlite

#endif
