#include <gtest/gtest.h>

#include <sqlgen.hpp>
#include <sqlgen/transpilation/to_create_table.hpp>

namespace test_tablename {

struct TestTable {
  static constexpr const char* tablename = "TEST_TABLE";

  std::string field1;
  int32_t field2;
  sqlgen::PrimaryKey<uint32_t> id;
  std::optional<std::string> nullable;
};

TEST(general, test_tablename) {
  const auto create_table_stmt =
      sqlgen::transpilation::to_create_table<TestTable>();

  EXPECT_EQ(create_table_stmt.table.name, "TEST_TABLE");
}
}  // namespace test_tablename
