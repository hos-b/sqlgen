#include "sqlgen/postgres/Connection.hpp"

#include <ranges>
#include <rfl.hpp>
#include <sstream>
#include <stdexcept>

#include "sqlgen/internal/collect/vector.hpp"
#include "sqlgen/internal/strings/strings.hpp"

namespace sqlgen::postgres {

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

}  // namespace sqlgen::postgres
