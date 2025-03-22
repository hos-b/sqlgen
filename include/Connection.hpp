#ifndef SQLGEN_CONNECTION_HPP_
#define SQLGEN_CONNECTION_HPP_

#include <optional>
#include <string>
#include <vector>

#include "Iterator.hpp"
#include "Ref.hpp"
#include "Result.hpp"
#include "dynamic/CreateTable.hpp"
#include "dynamic/Insert.hpp"
#include "dynamic/SelectFrom.hpp"

namespace sqlgen {

/// Abstract base class to be implemented by the different
/// database connections.
struct Connection {
  /// Executes a statement.
  /// TODO: Abstract away the different statements using rfl::TaggedUnion.
  virtual Result<Nothing> execute(const CreateTable& _stmt) = 0;

  /// Reads the results of a SelectFrom statement.
  virtual Result<Ref<Iterator>> read(const dynamic::SelectFrom& _query) = 0;

  /// Writes data into a table. Each vector in data MUST have the same length as
  /// _stmt.columns.
  virtual Result<Nothing> write(
      const dynamic::Insert& _stmt,
      const std::vector<std::vector<std::optional<std::string>>>& _data) = 0;
};

}  // namespace sqlgen

#endif
