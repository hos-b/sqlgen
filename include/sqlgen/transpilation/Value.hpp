#ifndef SQLGEN_TRANSPILATION_VALUE_HPP_
#define SQLGEN_TRANSPILATION_VALUE_HPP_

namespace sqlgen::transpilation {

template <class T>
struct Value {
  T val;
};

}  // namespace sqlgen::transpilation

#endif
