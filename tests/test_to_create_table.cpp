#include <gtest/gtest.h>

#include <rfl/json.hpp>
#include <sqlgen.hpp>
#include <sqlgen/parsing/to_create_table.hpp>

namespace test_to_create_table {

struct TestTable {
  std::string field1;
  int32_t field2;
  sqlgen::PrimaryKey<uint32_t> id;
  std::optional<std::string> nullable;
};

TEST(general, test_to_create_table) {
  const auto create_table_stmt = sqlgen::parsing::to_create_table<TestTable>();

  const std::string expected =
      R"({"table":{"alias":"","name":"TestTable","schema":""},"columns":[{"alias":"","name":"field1","type":{"type":"Text","properties":{"primary":false,"nullable":false}}},{"alias":"","name":"field2","type":{"type":"Int32","properties":{"primary":false,"nullable":false}}},{"alias":"","name":"id","type":{"type":"UInt32","properties":{"primary":true,"nullable":false}}},{"alias":"","name":"nullable","type":{"type":"Text","properties":{"primary":false,"nullable":true}}}],"if_not_exists":true})";

  const auto json_str = rfl::json::write(create_table_stmt);

  EXPECT_EQ(json_str, expected);
}
}  // namespace test_to_create_table
