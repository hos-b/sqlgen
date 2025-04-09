#include <gtest/gtest.h>

#include <rfl/json.hpp>
#include <sqlgen.hpp>
#include <sqlgen/parsing/to_select_from.hpp>

namespace test_to_select_from {

struct TestTable {
  std::string field1;
  int32_t field2;
  sqlgen::PrimaryKey<uint32_t> id;
  std::optional<std::string> nullable;
};

TEST(general, test_to_select_from) {
  const auto select_from_stmt = sqlgen::parsing::to_select_from<TestTable>();

  const std::string expected =
      R"({"table":{"name":"TestTable"},"columns":[{"name":"field1","type":{"type":"Text","properties":{"primary":false,"nullable":false}}},{"name":"field2","type":{"type":"Int32","properties":{"primary":false,"nullable":false}}},{"name":"id","type":{"type":"UInt32","properties":{"primary":true,"nullable":false}}},{"name":"nullable","type":{"type":"Text","properties":{"primary":false,"nullable":true}}}]})";

  const auto json_str = rfl::json::write(select_from_stmt);

  EXPECT_EQ(json_str, expected);
}
}  // namespace test_to_select_from
