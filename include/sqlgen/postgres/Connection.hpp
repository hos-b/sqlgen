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
#include "exec.hpp"

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
    return exec(conn_, _sql).transform([](auto&&) { return Nothing{}; });
  }

  Result<Ref<IteratorBase>> read(const dynamic::SelectFrom& _query) final {
    return error("TODO");
  }

  std::string to_sql(const dynamic::Statement& _stmt) noexcept final;

  Result<Nothing> start_write(const dynamic::Insert& _stmt) final {
    return execute(to_sql(_stmt));
  }

  Result<Nothing> end_write() final;

  Result<Nothing> write(
      const std::vector<std::vector<std::optional<std::string>>>& _data) final;

 private:
  std::string add_not_null_if_necessary(
      const dynamic::types::Properties& _p) const noexcept;

  std::string column_or_value_to_sql(
      const dynamic::ColumnOrValue& _col) const noexcept;

  std::string column_to_sql_definition(
      const dynamic::Column& _col) const noexcept;

  std::string condition_to_sql(
      const dynamic::Condition& _condition) const noexcept;

  template <class ConditionType>
  std::string condition_to_sql_impl(
      const ConditionType& _condition) const noexcept;

  std::string create_table_to_sql(
      const dynamic::CreateTable& _stmt) const noexcept;

  static std::string get_name(const dynamic::Column& _col) { return _col.name; }

  std::vector<std::string> get_primary_keys(
      const dynamic::CreateTable& _stmt) const noexcept;

  std::string insert_to_sql(const dynamic::Insert& _stmt) const noexcept;

  static ConnPtr make_conn(const std::string& _conn_str);

  std::string select_from_to_sql(
      const dynamic::SelectFrom& _stmt) const noexcept;

  std::string to_buffer(
      const std::vector<std::optional<std::string>>& _line) const noexcept;

  std::string type_to_sql(const dynamic::Type& _type) const noexcept;

  static std::string wrap_in_quotes(const std::string& _name) noexcept {
    return "\"" + _name + "\"";
  }

 private:
  ConnPtr conn_;

  Credentials credentials_;
};

template <class ConditionType>
std::string Connection::condition_to_sql_impl(
    const ConditionType& _condition) const noexcept {
  using C = std::remove_cvref_t<ConditionType>;
  std::stringstream stream;

  if constexpr (std::is_same_v<C, dynamic::Condition::And>) {
    stream << "(" << condition_to_sql(*_condition.cond1) << ") AND ("
           << condition_to_sql(*_condition.cond2) << ")";

  } else if constexpr (std::is_same_v<C, dynamic::Condition::Equal>) {
    stream << column_or_value_to_sql(_condition.op1) << " = "
           << column_or_value_to_sql(_condition.op2);

  } else if constexpr (std::is_same_v<C, dynamic::Condition::GreaterEqual>) {
    stream << column_or_value_to_sql(_condition.op1)
           << " >= " << column_or_value_to_sql(_condition.op2);

  } else if constexpr (std::is_same_v<C, dynamic::Condition::GreaterThan>) {
    stream << column_or_value_to_sql(_condition.op1) << " > "
           << column_or_value_to_sql(_condition.op2);

  } else if constexpr (std::is_same_v<C, dynamic::Condition::NotEqual>) {
    stream << column_or_value_to_sql(_condition.op1)
           << " != " << column_or_value_to_sql(_condition.op2);

  } else if constexpr (std::is_same_v<C, dynamic::Condition::LesserEqual>) {
    stream << column_or_value_to_sql(_condition.op1)
           << " <= " << column_or_value_to_sql(_condition.op2);

  } else if constexpr (std::is_same_v<C, dynamic::Condition::LesserThan>) {
    stream << column_or_value_to_sql(_condition.op1) << " < "
           << column_or_value_to_sql(_condition.op2);

  } else if constexpr (std::is_same_v<C, dynamic::Condition::Or>) {
    stream << "(" << condition_to_sql(*_condition.cond1) << ") OR ("
           << condition_to_sql(*_condition.cond2) << ")";

  } else {
    static_assert(rfl::always_false_v<C>, "Not all cases where covered.");
  }

  return stream.str();
}

}  // namespace sqlgen::postgres

#endif
