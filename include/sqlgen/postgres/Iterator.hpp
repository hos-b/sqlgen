#ifndef SQLGEN_POSTGRES_ITERATOR_HPP_
#define SQLGEN_POSTGRES_ITERATOR_HPP_

#include <sqlite3.h>

#include <optional>
#include <string>
#include <vector>

#include "../IteratorBase.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "Connection.hpp"

namespace sqlgen::postgres {

class Iterator : public sqlgen::IteratorBase {
 public:
  Iterator(const StmtPtr& _stmt, const ConnPtr& _conn);

  ~Iterator();

  /// Whether the end of the available data has been reached.
  bool end() const final;

  /// Returns the next batch of rows.
  /// If _batch_size is greater than the number of rows left, returns all
  /// of the rows left.
  Result<std::vector<std::vector<std::optional<std::string>>>> next(
      const size_t _batch_size) final;

 private:
};

}  // namespace sqlgen::postgres

#endif
