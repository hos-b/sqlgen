#ifndef SQLGEN_POSTGRES_CONNECTION_HPP_
#define SQLGEN_POSTGRES_CONNECTION_HPP_

#include <libpq-fe.h>

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
#include "Credentials.hpp"

namespace sqlgen::postgres {

class Connection : public sqlgen::Connection {
  using ConnPtr = Ref<PGconn>;

 public:
  Connection(const Credentials& _credentials)
      : conn_(make_conn(_credentials.to_str())), credentials_(_credentials) {}

  static rfl::Result<Ref<sqlgen::Connection>> make(
      const Credentials& _credentials) noexcept;

  ~Connection() = default;

  Result<Nothing> commit() final { return execute("COMMIT;"); }

  Result<Nothing> execute(const std::string& _sql) noexcept final {
    return error("TODO");
  }

  Result<Ref<IteratorBase>> read(const dynamic::SelectFrom& _query) final {
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
  static ConnPtr make_conn(const std::string& _conn_str);

  std::string select_from_to_sql(const dynamic::SelectFrom& _stmt) noexcept;

 private:
  ConnPtr conn_;

  Credentials credentials_;
};

}  // namespace sqlgen::postgres

#endif
