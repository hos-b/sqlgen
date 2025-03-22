#ifndef SQLGEN_DYNAMIC_CREATE_TABLE_HPP_
#define SQLGEN_DYNAMIC_CREATE_TABLE_HPP_

#include <string>
#include <vector>

#include "Column.hpp"
#include "CreateTable.hpp"
#include "Table.hpp"

namespace sqlgen::dynamic {

CreateTable create_table(const std::string& _table,
                         const std::vector<Column>& _columns) {
  return CreateTable{.table = Table{.name = _table}, .columns = _columns};
}

CreateTable create_table(std::string& _schema, const std::string& _table,
                         const std::vector<Column>& _columns) {
  return CreateTable{.table = Table{.name = _table, .schema = _schema},
                     .columns = _columns};
}

}  // namespace sqlgen::dynamic

#endif
