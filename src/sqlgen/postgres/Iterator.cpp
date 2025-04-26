#include "sqlgen/postgres/Iterator.hpp"

#include <ranges>
#include <rfl.hpp>
#include <sstream>

#include "sqlgen/internal/collect/vector.hpp"
#include "sqlgen/internal/strings/strings.hpp"
#include "sqlgen/postgres/exec.hpp"

namespace sqlgen::postgres {

Iterator::Iterator(const std::string& _sql, const ConnPtr& _conn)
    : cursor_name_(make_cursor_name()), conn_(_conn), moved_(false) {
  exec(conn_, "BEGIN").value();
  exec(conn_, "DECLARE " + cursor_name_ + " CURSOR FOR " + _sql).value();
}

Iterator::Iterator(Iterator&& _other) noexcept
    : cursor_name_(std::move(_other.cursor_name_)),
      conn_(std::move(_other.conn_)),
      moved_(_other.moved_) {
  _other.moved_ = true;
}

Iterator::~Iterator() {
  if (!moved_) {
    exec(conn_, "CLOSE " + cursor_name_);
    exec(conn_, "END");
  }
}

bool Iterator::end() const { return true; }

Result<std::vector<std::vector<std::optional<std::string>>>> Iterator::next(
    const size_t _batch_size) {
  if (end()) {
    return error("End is reached.");
  }

  return error("TODO");
}

Iterator& Iterator::operator=(Iterator&& _other) noexcept {
  if (this == &_other) {
    return *this;
  }
  cursor_name_ = _other.cursor_name_;
  conn_ = _other.conn_;
  moved_ = _other.moved_;
  _other.moved_ = true;
  return *this;
}

}  // namespace sqlgen::postgres
