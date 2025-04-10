#ifndef SQLGEN_RANGE_HPP_
#define SQLGEN_RANGE_HPP_

#include <memory>
#include <ranges>

#include "Iterator.hpp"

namespace sqlgen {

/// This class is meant to provide a way to iterate through the data in the
/// database efficiently that is compatible with std::ranges.
template <class T>
class Range {
 public:
  using value_type = T;

  struct End {};

  Range(const Ref<IteratorBase>& _it) : it_(_it) {}

  ~Range() = default;

  auto begin() const { return Iterator<T>(it_); }

  auto end() const { return typename Iterator<T>::End{}; }

 private:
  /// The underlying database iterator.
  Ref<IteratorBase> it_;
};

}  // namespace sqlgen

#endif
