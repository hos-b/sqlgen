#ifndef SQLGEN_ITERATOR_HPP_
#define SQLGEN_ITERATOR_HPP_

#include <string>
#include <vector>

#include "Result.hpp"

namespace sqlgen {

/// Abstract base class for an iterator to be returned by Connection::read(...).
struct Iterator {
  /// Whether the end of the available data has been reached.
  virtual bool end() const = 0;

  /// Returns the next batch of rows.
  /// If _batch_size is greater than the number of rows left, returns all
  /// of the rows left.
  virtual Result<std::vector<std::vector<std::string>>> next(
      const size_t _batch_size) = 0;
};

}  // namespace sqlgen

#endif
