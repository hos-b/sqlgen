#include <gtest/gtest.h>

#include <rfl/json.hpp>
#include <sqlgen.hpp>
#include <sqlgen/transpilation/to_insert.hpp>

namespace test_to_insert {

struct TestTable {
  std::string field1;
  int32_t field2;
  sqlgen::PrimaryKey<uint32_t> id;
  std::optional<std::string> nullable;
};

TEST(general, test_to_insert) {
  const auto insert_stmt = sqlgen::transpilation::to_insert<TestTable>();

  const std::string expected =
      R"({"table":{"name":"TestTable"},"columns":["field1","field2","id","nullable"]})";

  const auto json_str = rfl::json::write(insert_stmt);

  EXPECT_EQ(json_str, expected);
}
}  // namespace test_to_insert
