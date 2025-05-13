#include "sqlgen/postgres/Connection.hpp"

#include <ranges>
#include <rfl.hpp>
#include <sstream>
#include <stdexcept>

#include "sqlgen/internal/collect/vector.hpp"
#include "sqlgen/internal/strings/strings.hpp"
#include "sqlgen/postgres/Iterator.hpp"

namespace sqlgen::postgres {

Connection::Connection(Connection&& _other) noexcept
    : conn_(std::move(_other.conn_)),
      credentials_(std::move(_other.credentials_)),
      transaction_started_(_other.transaction_started_) {
  _other.transaction_started_ = false;
}

Connection::~Connection() {
  if (transaction_started_) {
    rollback();
  }
}

Result<Nothing> Connection::begin_transaction() noexcept {
  if (transaction_started_) {
    return error(
        "Cannot BEGIN TRANSACTION - another transaction has been started.");
  }
  transaction_started_ = true;
  return execute("BEGIN TRANSACTION;");
}

Result<Nothing> Connection::commit() noexcept {
  if (!transaction_started_) {
    return error("Cannot COMMIT - no transaction has been started.");
  }
  transaction_started_ = false;
  return execute("COMMIT;");
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

Connection& Connection::operator=(Connection&& _other) noexcept {
  if (this == &_other) {
    return *this;
  }
  if (transaction_started_) {
    rollback();
  }
  conn_ = std::move(_other.conn_);
  credentials_ = std::move(_other.credentials_);
  transaction_started_ = _other.transaction_started_;
  _other.transaction_started_ = false;
  return *this;
}

Result<Ref<IteratorBase>> Connection::read(const dynamic::SelectFrom& _query) {
  const auto sql = postgres::to_sql_impl(_query);
  try {
    return Ref<IteratorBase>(Ref<Iterator>::make(sql, conn_));
  } catch (std::exception& e) {
    return error(e.what());
  }
}

Result<Nothing> Connection::rollback() noexcept {
  if (!transaction_started_) {
    return error("Cannot ROLLBACK - no transaction has been started.");
  }
  transaction_started_ = false;
  return execute("ROLLBACK;");
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
