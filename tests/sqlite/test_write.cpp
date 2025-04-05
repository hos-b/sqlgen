#include <gtest/gtest.h>

#include <sqlgen.hpp>
#include <sqlgen/sqlite.hpp>
#include <vector>

namespace test_write {

struct Person {
  sqlgen::PrimaryKey<uint32_t> id;
  std::string first_name;
  std::string last_name;
  int age;
};

TEST(sqlite, test_write) {
  const auto conn = sqlgen::sqlite::connect().value();

  const auto people = std::vector<Person>(
      {Person{
           .id = 0, .first_name = "Homer", .last_name = "Simpson", .age = 45},
       Person{.id = 1, .first_name = "Bart", .last_name = "Simpson", .age = 10},
       Person{.id = 2, .first_name = "Lisa", .last_name = "Simpson", .age = 8},
       Person{
           .id = 3, .first_name = "Maggie", .last_name = "Simpson", .age = 0}});

  sqlgen::write(conn, people.begin(), people.end()).value();
}

}  // namespace test_write
