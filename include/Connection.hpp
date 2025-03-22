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

  /// Starts the write operation.
  virtual Result<Nothing> start_write(const dynamic::Insert& _stmt) = 0;

  /// Ends the write operation and thus commits the results.
  virtual Result<Nothing> end_write() = 0;

  /// Writes data into a table. Each vector in data MUST have the same length as
  /// _stmt.columns.
  /// You MUST call .start_write(...) first and call .end_write() after all
  /// the data has been written.
  /// You CAN write the data in chunks, meaning you can call .write(...) more
  /// than once between .start_write(...) and .end_write().
  virtual Result<Nothing> write(
      const std::vector<std::vector<std::optional<std::string>>>& _data) = 0;
};

}  // namespace sqlgen

#endif
