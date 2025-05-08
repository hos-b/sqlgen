#ifndef SQLGEN_SQLITE_TO_SQL_HPP_
#define SQLGEN_SQLITE_TO_SQL_HPP_

#include <string>

#include "../dynamic/Statement.hpp"

namespace sqlgen::sqlite {

/// Transpiles a dynamic general SQL statement to the sqlite dialect.
std::string to_sql(const dynamic::Statement& _stmt) noexcept;

}  // namespace sqlgen::sqlite

#endif
