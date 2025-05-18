#ifndef SQLGEN_CREATEINDEX_HPP_
#define SQLGEN_CREATEINDEX_HPP_

#include <rfl.hpp>

#include "Ref.hpp"
#include "Result.hpp"
#include "transpilation/columns_t.hpp"
#include "transpilation/to_create_index.hpp"

namespace sqlgen {

template <class ValueType, class ColumnsType, class WhereType>
Result<Ref<Connection>> create_index_impl(const Ref<Connection>& _conn,
                                          const std::string& _name,
                                          const bool _unique,
                                          const bool _if_not_exists,
                                          const WhereType& _where) {
  const auto query =
      transpilation::to_create_index<ValueType, ColumnsType, WhereType>(
          _name, _unique, _if_not_exists, _where);
  return _conn->execute(_conn->to_sql(query)).transform([&](const auto&) {
    return _conn;
  });
}

template <class ValueType, class ColumnsType, class WhereType>
Result<Ref<Connection>> create_index_impl(const Result<Ref<Connection>>& _res,
                                          const std::string& _name,
                                          const bool _unique,
                                          const bool _if_not_exists,
                                          const WhereType& _where) {
  return _res.and_then([&](const auto& _conn) {
    return create_index_impl<ValueType, ColumnsType, WhereType>(
        _conn, _name, _unique, _if_not_exists, _where);
  });
}

template <rfl::internal::StringLiteral _name, class ValueType, class WhereType,
          class... ColTypes>
struct CreateIndex {
  Result<Ref<Connection>> operator()(const auto& _conn) const noexcept {
    try {
      return create_index_impl<ValueType,
                               transpilation::columns_t<ValueType, ColTypes...>,
                               WhereType>(_conn, _name.str(), unique_,
                                          if_not_exists_, where_);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

  bool unique_ = false;
  bool if_not_exists_ = false;

  WhereType where_;
};

template <rfl::internal::StringLiteral _name, class ValueType,
          class... ColTypes>
auto create_index(const ColTypes&...) {
  return CreateIndex<_name, ValueType, Nothing, ColTypes...>{
      .unique_ = false, .if_not_exists_ = false};
}

template <rfl::internal::StringLiteral _name, class ValueType,
          class... ColTypes>
auto create_unique_index(const ColTypes&...) {
  return CreateIndex<_name, ValueType, Nothing, ColTypes...>{
      .unique_ = true, .if_not_exists_ = false};
}

};  // namespace sqlgen

#endif

