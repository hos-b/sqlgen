#ifndef SQLGEN_TRANSPILATION_VALUET_HPP_
#define SQLGEN_TRANSPILATION_VALUET_HPP_

#include "../Range.hpp"

namespace sqlgen::transpilation {

template <class ContainerType>
struct ValueType;

template <class ContainerType>
struct ValueType {
  using Type = typename ContainerType::value_type;
};

template <class T>
struct ValueType<Range<T>> {
  using Type = typename Range<T>::value_type::value_type;
};

template <class T>
using value_t = typename ValueType<T>::Type;

}  // namespace sqlgen::transpilation

#endif
