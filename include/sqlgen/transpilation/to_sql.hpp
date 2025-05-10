#ifndef SQLGEN_TRANSPILATION_TO_SQL_HPP_
#define SQLGEN_TRANSPILATION_TO_SQL_HPP_

#include <vector>

#include "../CreateTable.hpp"
#include "../Insert.hpp"
#include "../delete_from.hpp"
#include "../drop.hpp"
#include "../dynamic/Statement.hpp"
#include "../read.hpp"
#include "to_create_table.hpp"
#include "to_delete_from.hpp"
#include "to_drop.hpp"
#include "to_insert.hpp"
#include "to_select_from.hpp"
#include "value_t.hpp"

namespace sqlgen::transpilation {

template <class T>
struct ToSQL;

template <class T>
struct ToSQL<CreateTable<T>> {
  dynamic::Statement operator()(const auto&) const {
    return to_create_table<T>();
  }
};

template <class T, class WhereType>
struct ToSQL<DeleteFrom<T, WhereType>> {
  dynamic::Statement operator()(const auto& _delete_from) const {
    return to_delete_from<T>(_delete_from.where_);
  }
};

template <class T>
struct ToSQL<Drop<T>> {
  dynamic::Statement operator()(const auto& _drop) const {
    return to_drop<T>(_drop.if_exists_);
  }
};

template <class T>
struct ToSQL<Insert<T>> {
  dynamic::Statement operator()(const auto&) const { return to_insert<T>(); }
};

template <class ContainerType, class WhereType, class OrderByType,
          class LimitType>
struct ToSQL<Read<ContainerType, WhereType, OrderByType, LimitType>> {
  dynamic::Statement operator()(const auto& _read) const {
    return to_select_from<value_t<ContainerType>, WhereType, OrderByType,
                          LimitType>(_read.where_, _read.limit_);
  }
};

template <class T>
dynamic::Statement to_sql(const T& _t) {
  return ToSQL<std::remove_cvref_t<T>>{}(_t);
}

}  // namespace sqlgen::transpilation

#endif
