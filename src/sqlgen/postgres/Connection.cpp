#include "sqlgen/postgres/Connection.hpp"

#include <ranges>
#include <rfl.hpp>
#include <sstream>
#include <stdexcept>

#include "sqlgen/internal/collect/vector.hpp"
#include "sqlgen/internal/strings/strings.hpp"

namespace sqlgen::postgres {

std::string Connection::add_not_null_if_necessary(
    const dynamic::types::Properties& _p) const noexcept {
  return std::string(_p.nullable ? "" : " NOT NULL");
}

std::string Connection::column_to_sql_definition(
    const dynamic::Column& _col) const noexcept {
  return wrap_in_quotes(_col.name) + " " + type_to_sql(_col.type) +
         add_not_null_if_necessary(
             _col.type.visit([](const auto& _t) { return _t.properties; }));
}

std::string Connection::create_table_to_sql(
    const dynamic::CreateTable& _stmt) const noexcept {
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
    stream << wrap_in_quotes(*_stmt.table.schema) << ".";
  }
  stream << wrap_in_quotes(_stmt.table.name) << " ";

  stream << "(";
  stream << internal::strings::join(
      ", ", internal::collect::vector(_stmt.columns | transform(col_to_sql)));

  const auto primary_keys = get_primary_keys(_stmt);

  if (primary_keys.size() != 0) {
    stream << ", PRIMARY KEY (" << internal::strings::join(", ", primary_keys)
           << ")";
  }

  stream << ");";

  return stream.str();
}

Result<Nothing> Connection::end_write() {
  if (PQputCopyEnd(conn_.get(), NULL) == -1) {
    return error(PQerrorMessage(conn_.get()));
  }
  const auto res = PQgetResult(conn_.get());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    return error(PQerrorMessage(conn_.get()));
  }
  return Nothing{};
}

Result<Ref<PGresult>> Connection::exec(const std::string& _sql) const noexcept {
  const auto res = PQexec(conn_.get(), _sql.c_str());

  const auto status = PQresultStatus(res);

  if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK &&
      status != PGRES_COPY_IN) {
    const auto err = error(PQresultErrorMessage(res));
    PQclear(res);
    return err;
  }

  PQclear(res);

  return Ref<PGresult>::make(std::shared_ptr<PGresult>(res, PQclear));
}

std::vector<std::string> Connection::get_primary_keys(
    const dynamic::CreateTable& _stmt) const noexcept {
  using namespace std::ranges::views;

  const auto is_primary_key = [](const auto& _col) -> bool {
    return _col.type.visit(
        [](const auto& _t) -> bool { return _t.properties.primary; });
  };

  return internal::collect::vector(_stmt.columns | filter(is_primary_key) |
                                   transform(get_name) |
                                   transform(wrap_in_quotes));
}

std::string Connection::insert_to_sql(
    const dynamic::Insert& _stmt) const noexcept {
  using namespace std::ranges::views;
  const auto schema = wrap_in_quotes(_stmt.table.schema.value_or("public"));
  const auto table = wrap_in_quotes(_stmt.table.name);
  const auto colnames = internal::strings::join(
      ", ",
      internal::collect::vector(_stmt.columns | transform(wrap_in_quotes)));
  return "COPY " + schema + "." + table + "(" + colnames +
         ") FROM STDIN WITH DELIMITER '\t' NULL '\e' QUOTE '\a';";
}

rfl::Result<Ref<sqlgen::Connection>> Connection::make(
    const Credentials& _credentials) noexcept {
  try {
    return Ref<sqlgen::Connection>(Ref<Connection>::make(_credentials));
  } catch (std::exception& e) {
    return error(e.what());
  }
}

typename Connection::ConnPtr Connection::make_conn(
    const std::string& _conn_str) {
  const auto raw_ptr = PQconnectdb(_conn_str.c_str());

  if (PQstatus(raw_ptr) != CONNECTION_OK) {
    const auto msg = std::string("Connection to postgres failed: ") +
                     PQerrorMessage(raw_ptr);
    PQfinish(raw_ptr);
    throw std::runtime_error(msg.c_str());
  }

  return ConnPtr::make(std::shared_ptr<PGconn>(raw_ptr, &PQfinish)).value();
}

std::string Connection::select_from_to_sql(
    const dynamic::SelectFrom& _stmt) const noexcept {
  using namespace std::ranges::views;

  std::stringstream stream;
  stream << "SELECT ";
  stream << internal::strings::join(
      ", ", internal::collect::vector(_stmt.columns | transform(get_name) |
                                      transform(wrap_in_quotes)));
  stream << " FROM ";
  if (_stmt.table.schema) {
    stream << wrap_in_quotes(*_stmt.table.schema) << ".";
  }
  stream << wrap_in_quotes(_stmt.table.name) << ";";

  return stream.str();
}

std::string Connection::to_buffer(
    const std::vector<std::optional<std::string>>& _line) const noexcept {
  using namespace std::ranges::views;

  const auto edit_field =
      [](const std::optional<std::string>& _field) -> std::string {
    if (!_field) {
      return "\e";
    }
    if (_field->find("\t") != std::string::npos) {
      return "\a" + *_field + "\a";
    }
    return *_field;
  };

  return internal::strings::join(
             "\t", internal::collect::vector(_line | transform(edit_field))) +
         "\n";
}

std::string Connection::to_sql(const dynamic::Statement& _stmt) noexcept {
  return _stmt.visit([&](const auto& _s) -> std::string {
    using S = std::remove_cvref_t<decltype(_s)>;
    if constexpr (std::is_same_v<S, dynamic::CreateTable>) {
      return create_table_to_sql(_s);
    } else if constexpr (std::is_same_v<S, dynamic::Insert>) {
      return insert_to_sql(_s);
    } else if constexpr (std::is_same_v<S, dynamic::SelectFrom>) {
      return select_from_to_sql(_s);
    } else {
      static_assert(rfl::always_false_v<S>, "Unsupported type.");
    }
  });
}

std::string Connection::type_to_sql(const dynamic::Type& _type) const noexcept {
  return _type.visit([](const auto _t) -> std::string {
    using T = std::remove_cvref_t<decltype(_t)>;
    if constexpr (std::is_same_v<T, dynamic::types::Boolean>) {
      return "BOOLEAN";
    } else if constexpr (std::is_same_v<T, dynamic::types::Int8> ||
                         std::is_same_v<T, dynamic::types::Int16> ||
                         std::is_same_v<T, dynamic::types::UInt8> ||
                         std::is_same_v<T, dynamic::types::UInt16>) {
      return "SMALLINT";
    } else if constexpr (std::is_same_v<T, dynamic::types::Int32> ||
                         std::is_same_v<T, dynamic::types::UInt32>) {
      return "INTEGER";
    } else if constexpr (std::is_same_v<T, dynamic::types::Int64> ||
                         std::is_same_v<T, dynamic::types::UInt64>) {
      return "BIGINT";
    } else if constexpr (std::is_same_v<T, dynamic::types::Float32>) {
      return "REAL";
    } else if constexpr (std::is_same_v<T, dynamic::types::Float64>) {
      return "DOUBLE PRECISION";
    } else if constexpr (std::is_same_v<T, dynamic::types::Text>) {
      return "TEXT";
    } else if constexpr (std::is_same_v<T, dynamic::types::VarChar>) {
      return "VARCHAR(" + std::to_string(_t.length) + ")";
    } else if constexpr (std::is_same_v<T, dynamic::types::Timestamp>) {
      return "TIMESTAMP";
    } else if constexpr (std::is_same_v<T, dynamic::types::TimestampWithTZ>) {
      return "TIMESTAMP WITH TIME ZONE";
    } else if constexpr (std::is_same_v<T, dynamic::types::Unknown>) {
      return "TEXT";
    } else {
      static_assert(rfl::always_false_v<T>, "Not all cases were covered.");
    }
  });
}

Result<Nothing> Connection::write(
    const std::vector<std::vector<std::optional<std::string>>>& _data) {
  for (const auto& line : _data) {
    const auto buffer = to_buffer(line);
    const auto success = PQputCopyData(conn_.get(), buffer.c_str(),
                                       static_cast<int>(buffer.size()));
    if (success != 1) {
      PQputCopyEnd(conn_.get(), NULL);
      return error("Error occurred while writing data to postgres.");
    }
  }
  return Nothing{};
}

}  // namespace sqlgen::postgres
