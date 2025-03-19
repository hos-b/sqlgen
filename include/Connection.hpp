#ifndef SQLGEN_CONNECTION_HPP_
#define SQLGEN_CONNECTION_HPP_

#include <string>
#include <vector>

#include "Result.hpp"
#include "dynamic/Insert.hpp"
#include "dynamic/SelectFrom.hpp"

namespace sqlgen {

/// Abstract base class to be implemented by the different
/// database connections.
struct Connection {
  /// Reads the results of a SelectFrom statement.
  virtual Result<std::vector<std::vector<std::string>>> read(
      const dynamic::SelectFrom& _query) = 0;

  /// Writes data into a table. Each vector in data MUST have the same length as
  /// _stmt.columns.
  virtual Result<Nothing> write(
      const dynamic::Insert& _stmt,
      const std::vector<std::vector<std::string>>& _data) = 0;
};

}  // namespace sqlgen

#endif
