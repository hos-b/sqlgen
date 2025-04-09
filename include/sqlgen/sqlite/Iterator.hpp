#ifndef SQLGEN_SQLITE_ITERATOR_HPP_
#define SQLGEN_SQLITE_ITERATOR_HPP_

#include <sqlite3.h>

#include <optional>
#include <string>
#include <vector>

#include "../IteratorBase.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "Connection.hpp"

namespace sqlgen::sqlite {

class Iterator : public sqlgen::IteratorBase {
  using ConnPtr = Ref<sqlite3>;
  using StmtPtr = Ref<sqlite3_stmt>;

 public:
  Iterator(const StmtPtr& _stmt, const ConnPtr& _conn)
      : rownum_(0), stmt_(_stmt), conn_(_conn) {}

  ~Iterator() = default;

  /// Whether the end of the available data has been reached.
  bool end() const final { return false; }

  /// Returns the next batch of rows.
  /// If _batch_size is greater than the number of rows left, returns all
  /// of the rows left.
  Result<std::vector<std::vector<std::optional<std::string>>>> next(
      const size_t _batch_size) final {
    return error("TODO");
  }

 private:
  /// The current rownumber.
  size_t rownum_;

  /// The prepared statement. Note that we have
  /// declared it before conn_, meaning it will be destroyed first.
  StmtPtr stmt_;

  /// The underlying sqlite3 connection. We have this in here to prevent its
  /// destruction for the lifetime of the iterator.
  ConnPtr conn_;
};

}  // namespace sqlgen::sqlite

#endif
