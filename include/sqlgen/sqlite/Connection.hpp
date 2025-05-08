#ifndef SQLGEN_SQLITE_CONNECTION_HPP_
#define SQLGEN_SQLITE_CONNECTION_HPP_

#include <sqlite3.h>

#include <memory>
#include <rfl.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../Connection.hpp"
#include "../IteratorBase.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "to_sql.hpp"

namespace sqlgen::sqlite {

class Connection : public sqlgen::Connection {
  using ConnPtr = Ref<sqlite3>;
  using StmtPtr = std::shared_ptr<sqlite3_stmt>;

 public:
  Connection(const std::string& _fname)
      : stmt_(nullptr), conn_(make_conn(_fname)) {}

  static rfl::Result<Ref<sqlgen::Connection>> make(
      const std::string& _fname) noexcept;

  ~Connection() = default;

  Result<Nothing> commit() final { return execute("COMMIT;"); }

  Result<Nothing> execute(const std::string& _sql) noexcept final;

  Result<Ref<IteratorBase>> read(const dynamic::SelectFrom& _query) final;

  std::string to_sql(const dynamic::Statement& _stmt) noexcept final {
    return sqlite::to_sql(_stmt);
  }

  Result<Nothing> start_write(const dynamic::Insert& _stmt) final;

  Result<Nothing> end_write() final;

  Result<Nothing> write(
      const std::vector<std::vector<std::optional<std::string>>>& _data) final;

 private:
  /// Generates the underlying connection.
  static ConnPtr make_conn(const std::string& _fname);

 private:
  /// A prepared statement - needed for the read and write operations. Note that
  /// we have declared it before conn_, meaning it will be destroyed first.
  StmtPtr stmt_;

  /// The underlying sqlite3 connection.
  ConnPtr conn_;
};

}  // namespace sqlgen::sqlite

#endif
