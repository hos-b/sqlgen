#ifndef SQLGEN_POSTGRES_EXEC_HPP_
#define SQLGEN_POSTGRES_EXEC_HPP_

#include <libpq-fe.h>

#include <rfl.hpp>
#include <string>

#include "../Ref.hpp"
#include "../Result.hpp"

namespace sqlgen::postgres {

Result<Ref<PGresult>> exec(const Ref<PGconn>& _conn,
                           const std::string& _sql) noexcept;

}  // namespace sqlgen::postgres

#endif
