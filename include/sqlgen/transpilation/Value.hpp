#ifndef SQLGEN_TRANSPILATION_VALUE_HPP_
#define SQLGEN_TRANSPILATION_VALUE_HPP_

#include <type_traits>

namespace sqlgen::transpilation {

template <class T>
struct Value {
  T val;
};

template <class T>
auto make_value(T&& _t) {
  return Value<std::remove_cvref_t<T>>{.val_ = _t};
}

}  // namespace sqlgen::transpilation

#endif
