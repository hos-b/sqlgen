#include <gtest/gtest.h>

#include <sqlgen.hpp>
#include <sqlgen/parsing/to_create_table.hpp>

namespace test_schema {

struct TestTable {
  static constexpr const char* schema = "test";

  std::string field1;
  int32_t field2;
  sqlgen::PrimaryKey<uint32_t> id;
  std::optional<std::string> nullable;
};

TEST(general, test_schema) {
  const auto create_table_stmt = sqlgen::parsing::to_create_table<TestTable>();

  EXPECT_EQ(create_table_stmt.table.schema, "test");
}
}  // namespace test_schema
