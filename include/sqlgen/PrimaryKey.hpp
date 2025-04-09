#ifndef SQLGEN_PRIMARY_KEY_HPP_
#define SQLGEN_PRIMARY_KEY_HPP_

#include "transpilation/is_nullable.hpp"

namespace sqlgen {

template <class T>
struct PrimaryKey {
  using ReflectionType = T;

  static_assert(!transpilation::is_nullable_v<T>,
                "A primary key cannot be nullable.");

  PrimaryKey() : value_(0) {}

  PrimaryKey(const T& _value) : value_(_value) {}

  PrimaryKey(PrimaryKey<T>&& _other) noexcept = default;

  PrimaryKey(const PrimaryKey<T>& _other) = default;

  template <class U>
  PrimaryKey(const PrimaryKey<U>& _other) : value_(_other.get()) {}

  template <class U>
  PrimaryKey(PrimaryKey<U>&& _other) : value_(_other.get()) {}

  template <class U,
            typename std::enable_if<std::is_convertible_v<U, ReflectionType>,
                                    bool>::type = true>
  PrimaryKey(const U& _value) : value_(_value) {}

  template <class U,
            typename std::enable_if<std::is_convertible_v<U, ReflectionType>,
                                    bool>::type = true>
  PrimaryKey(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U,
            typename std::enable_if<std::is_convertible_v<U, ReflectionType>,
                                    bool>::type = true>
  PrimaryKey(const PrimaryKey<U>& _other) : value_(_other.value()) {}

  ~PrimaryKey() = default;

  /// Returns the underlying object.
  const ReflectionType& get() const { return value_; }

  /// Returns the underlying object.
  ReflectionType& operator()() { return value_; }

  /// Returns the underlying object.
  const ReflectionType& operator()() const { return value_; }

  /// Assigns the underlying object.
  auto& operator=(const ReflectionType& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object.
  template <class U,
            typename std::enable_if<std::is_convertible_v<U, ReflectionType>,
                                    bool>::type = true>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object.
  PrimaryKey<T>& operator=(const PrimaryKey<T>& _other) = default;

  /// Assigns the underlying object.
  PrimaryKey<T>& operator=(PrimaryKey<T>&& _other) = default;

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(const PrimaryKey<U>& _other) {
    value_ = _other.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(PrimaryKey<U>&& _other) {
    value_ = std::forward<T>(_other.value_);
    return *this;
  }

  /// Necessary for the automated transpilation to work.
  const T& reflection() const { return value_; }

  /// Assigns the underlying object.
  void set(const T& _value) { value_ = _value; }

  /// Returns the underlying object.
  T& value() { return value_; }

  /// Returns the underlying object.
  const T& value() const { return value_; }

  /// The underlying value.
  T value_;
};

}  // namespace sqlgen

#endif
