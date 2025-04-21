#include "sqlgen/postgres/Iterator.hpp"

#include <ranges>
#include <rfl.hpp>
#include <sstream>

#include "sqlgen/internal/collect/vector.hpp"
#include "sqlgen/internal/strings/strings.hpp"
#include "sqlgen/sqlite/Iterator.hpp"

namespace sqlgen::postgres {

Iterator::Iterator() {}

Iterator::~Iterator() = default;

bool Iterator::end() const { return true; }

Result<std::vector<std::vector<std::optional<std::string>>>> Iterator::next(
    const size_t _batch_size) {
  if (end()) {
    return error("End is reached.");
  }

  return error("TODO");
}

}  // namespace sqlgen::postgres
