#ifndef SQLGEN_SQLITE_CONNECTION_HPP_
#define SQLGEN_SQLITE_CONNECTION_HPP_

#include <sqlite3.h>

#include <memory>
#include <rfl.hpp>
#include <stdexcept>
#include <string>

#include "../Connection.hpp"
#include "../IteratorBase.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../dynamic/Column.hpp"
#include "../dynamic/Statement.hpp"

namespace sqlgen::sqlite {

class Connection : public sqlgen::Connection {
  using ConnPtr = std::unique_ptr<sqlite3, decltype(&sqlite3_close)>;
  using StmtPtr = std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>;

 public:
  Connection(const std::string& _fname)
      : stmt_(StmtPtr(nullptr, &sqlite3_finalize)), conn_(make_conn(_fname)) {}

  Connection(const Connection& _other) = delete;

  static rfl::Result<Ref<sqlgen::Connection>> make(
      const std::string& _fname) noexcept;

  ~Connection() = default;

  Result<Nothing> commit() final { return execute("COMMIT;"); }

  Result<Nothing> execute(const std::string& _sql) noexcept final;

  Connection& operator=(const Connection& _other) = delete;

  Result<Ref<IteratorBase>> read(const dynamic::SelectFrom& _query) final {
    return error("TODO");
  }

  std::string to_sql(const dynamic::Statement& _stmt) noexcept final;

  Result<Nothing> start_write(const dynamic::Insert& _stmt) final;

  Result<Nothing> end_write() final;

  Result<Nothing> write(
      const std::vector<std::vector<std::optional<std::string>>>& _data) final;

 private:
  /// Transforms a dynamic::Column to an SQL string that defines the column in
  /// a CREATE TABLE statement.
  std::string column_to_sql_definition(const dynamic::Column& _col) noexcept;

  /// Transforms a CreateTable Statement to an SQL string.
  std::string create_table_to_sql(const dynamic::CreateTable& _stmt) noexcept;

  /// Transforms an Insert Statement to an SQL string.
  std::string insert_to_sql(const dynamic::Insert& _stmt) noexcept;

  /// Generates the underlying connection.
  static ConnPtr make_conn(const std::string& _fname);

  /// Expresses the properies as SQL.
  std::string properties_to_sql(const dynamic::types::Properties& _p) noexcept;

  /// Expresses the type as SQL.
  std::string type_to_sql(const dynamic::Type& _type) noexcept;

 private:
  /// A prepared statement - needed for the write operations. Note that we have
  /// declared it before conn_, meaning it will be destroyed first.
  StmtPtr stmt_;

  /// The underlying sqlite3 connection.
  ConnPtr conn_;
};

}  // namespace sqlgen::sqlite

#endif
