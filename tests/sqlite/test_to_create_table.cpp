#include <gtest/gtest.h>

#include <sqlgen.hpp>
#include <sqlgen/parsing/to_create_table.hpp>
#include <sqlgen/sqlite.hpp>

namespace test_to_create_table {

struct TestTable {
  std::string field1;
  int32_t field2;
  sqlgen::PrimaryKey<uint32_t> id;
  std::optional<std::string> nullable;
};

TEST(sqlite, test_to_create_table) {
  const auto create_table_stmt = sqlgen::parsing::to_create_table<TestTable>();
  const auto conn = sqlgen::sqlite::connect().value();
  const auto expected =
      R"(CREATE TABLE "TestTable" IF NOT EXISTS ("field1" TEXT NOT NULL, "field2" INTEGER NOT NULL, "id" INTEGER PRIMARY KEY NOT NULL, "nullable" TEXT);)";

  EXPECT_EQ(conn->to_sql(create_table_stmt), expected);
}
}  // namespace test_to_create_table
