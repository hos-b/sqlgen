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
#include "../dynamic/Column.hpp"
#include "../dynamic/Statement.hpp"

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

  std::string to_sql(const dynamic::Statement& _stmt) noexcept final;

  Result<Nothing> start_write(const dynamic::Insert& _stmt) final;

  Result<Nothing> end_write() final;

  Result<Nothing> write(
      const std::vector<std::vector<std::optional<std::string>>>& _data) final;

 private:
  /// Transforms a column or value to SQL.
  std::string column_or_value_to_sql(
      const dynamic::ColumnOrValue& _col) const noexcept;

  /// Transforms a dynamic::Column to an SQL string that defines the column in
  /// a CREATE TABLE statement.
  std::string column_to_sql_definition(const dynamic::Column& _col) noexcept;

  /// Transforms a condition to SQL.
  std::string condition_to_sql(
      const dynamic::Condition& _condition) const noexcept;

  /// Transforms a partilar condition to SQL.
  template <class ConditionType>
  std::string condition_to_sql_impl(
      const ConditionType& _condition) const noexcept;

  /// Transforms a CreateTable Statement to an SQL string.
  std::string create_table_to_sql(const dynamic::CreateTable& _stmt) noexcept;

  /// Transforms an Insert Statement to an SQL string.
  std::string insert_to_sql(const dynamic::Insert& _stmt) noexcept;

  /// Generates the underlying connection.
  static ConnPtr make_conn(const std::string& _fname);

  /// Expresses the properies as SQL.
  std::string properties_to_sql(const dynamic::types::Properties& _p) noexcept;

  /// Transforms an Insert Statement to an SQL string.
  std::string select_from_to_sql(const dynamic::SelectFrom& _stmt) noexcept;

  /// Expresses the type as SQL.
  std::string type_to_sql(const dynamic::Type& _type) noexcept;

 private:
  /// A prepared statement - needed for the read and write operations. Note that
  /// we have declared it before conn_, meaning it will be destroyed first.
  StmtPtr stmt_;

  /// The underlying sqlite3 connection.
  ConnPtr conn_;
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

}  // namespace sqlgen::sqlite

#endif
