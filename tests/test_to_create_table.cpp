#include <gtest/gtest.h>

#include <sqlgen.hpp>
#include <sqlgen/parsing/to_create_table.hpp>

TEST(general, test_to_create_table) {
  struct TestTable {
    std::string field1;
    int32_t field2;
    sqlgen::PrimaryKey<uint32_t> id;
    std::optional<std::string> nullable;
  };

  const auto create_table_stmt = sqlgen::parsing::to_create_table<TestTable>();

  const auto get_properties = [](const auto& _t) { return _t.properties; };

  EXPECT_EQ(create_table_stmt.table.name, "TestTable");

  EXPECT_EQ(create_table_stmt.columns.size(), 4);

  EXPECT_EQ(create_table_stmt.columns.at(0).name, "field1");
  EXPECT_EQ(create_table_stmt.columns.at(1).name, "field2");
  EXPECT_EQ(create_table_stmt.columns.at(2).name, "id");
  EXPECT_EQ(create_table_stmt.columns.at(3).name, "nullable");

  EXPECT_EQ(create_table_stmt.columns.at(0).type.visit(get_properties).primary,
            false);
  EXPECT_EQ(create_table_stmt.columns.at(0).type.visit(get_properties).nullable,
            false);
  EXPECT_EQ(create_table_stmt.columns.at(2).type.visit(get_properties).primary,
            true);
  EXPECT_EQ(create_table_stmt.columns.at(3).type.visit(get_properties).nullable,
            true);
}
