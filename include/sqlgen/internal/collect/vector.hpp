#ifndef SQLGEN_INTERNAL_COLLECT_VECTOR_HPP_
#define SQLGEN_INTERNAL_COLLECT_VECTOR_HPP_

#include <ranges>

namespace sqlgen::internal::collect {

template <class RangeType>
auto vector(RangeType _r) {
  using T = std::ranges::range_value_t<RangeType>;
  std::vector<T> res;
  for (auto e : _r) {
    res.emplace_back(std::move(e));
  }
  return res;
}

}  // namespace sqlgen::internal::collect

#endif
