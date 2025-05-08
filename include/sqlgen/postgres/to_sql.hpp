#ifndef SQLGEN_POSTGRES_TO_SQL_HPP_
#define SQLGEN_POSTGRES_TO_SQL_HPP_

#include <string>

#include "../dynamic/Statement.hpp"

namespace sqlgen::postgres {

/// Transpiles a dynamic general SQL statement to the postgres dialect.
std::string to_sql(const dynamic::Statement& _stmt) noexcept;

}  // namespace sqlgen::postgres

#endif
