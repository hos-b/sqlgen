#ifndef SQLGEN_TRANSPILATION_DESC_HPP_
#define SQLGEN_TRANSPILATION_DESC_HPP_

namespace sqlgen::transpilation {

template <class _ColType>
struct Desc {
  using ColType = _ColType;
};

}  // namespace sqlgen::transpilation

#endif
