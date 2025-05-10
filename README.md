# sqlgen

sqlgen is an ORM and SQL query generator for C++-20, similar to Python's [SQLAlchemy](https://github.com/sqlalchemy/sqlalchemy)/[SQLModel](https://github.com/fastapi/sqlmodel) or Rust's [Diesel](https://github.com/diesel-rs/diesel).

Much like SQLModel is closely integrated with [pydantic](https://github.com/pydantic/pydantic),
sqlgen is closely integrated with our sister project [reflect-cpp](https://github.com/getml/reflect-cpp). This allows you to construct very reliable and highly efficient data pipelines.

## Inserting data 

Here is how you connect to a PostgreSQL database
and insert some data:

```cpp
#include <sqlgen/postgres.hpp>

struct People {
    std::string first_name;
    std::string last_name;
    uint age;
};

const auto people = std::vector<People>({
    People{.first_name = "Homer",
           .last_name = "Simpson",
           .age = 45}});

const auto credentials = sqlgen::postgres::Credentials{
    .user = "...", .password = "...", ...
};

const auto conn = sqlgen::postgres::connect(credentials);

// Will automatically create a table called 'People'
// with the columns 'first_name', 'last_name' and 'age', 
// if necessary.
const auto result = sqlgen::write(conn, people);

if (!result) {
    std::cout << result.error().what() << std::endl;
}
```

The resulting SQL code (will vary from dialect to dialect):

```sql
CREATE TABLE IF NOT EXISTS "People" (
    "first_name" TEXT NOT NULL,
    "last_name" TEXT NOT NULL,
    "age" INTEGER NOT NULL
);

INSERT INTO "People" ("first_name", "last_name", "age") VALUES (?, ?, ?);
```

## Retrieving data 

Here is how you retrieve the same data from the database
and print the results as a JSON:

```cpp
#include <rfl/json.hpp> // reflect-cpp
#include <sqlgen/postgres.hpp>

const auto conn = sqlgen::postgres::connect(credentials);

using namespace sqlgen;

// Query that returns the 100 youngest children.
// Columns are referred to using the _c operator.
const auto get_children = sqlgen::read<std::vector<People>> |
                          where("age"_c < 18) |
                          order_by("age"_c, "first_name"_c) |
                          limit(100);

// Actually executes the query.
// Returns sqlgen::Result<std::vector<People>>
const auto result = get_children(conn);

if (result) {
    std::cout << rfl::json::write(*result) << std::endl;
} else {
    std::cout << result.error().what() << std::endl;
}
```

The resulting SQL code:

```sql
SELECT "first_name", "last_name", "age"
FROM "People"
WHERE "age" < 18
ORDER BY "age", "first_name" 
LIMIT 100;
```

## Compile-time checks

sqlgen protects you from various mistakes through comprehensive
compile time checks:

```cpp
struct People {
    std::string first_name;
    std::string last_name;
    uint age;
};

// Will not compile - no column named "color".
const auto get_children = sqlgen::read<std::vector<People>> |
                          where("age"_c < 18 and "color"_c != 'green');
```

## Protection against SQL injections

sqlgen provides input validation to protect against SQL injection.

```cpp
// Safe query function using AlphaNumeric for filtering
std::vector<People> get_people(const auto& conn, 
                               const sqlgen::AlphaNumeric& first_name) {
    using namespace sqlgen;
    const auto query = sqlgen::read<std::vector<People>> | 
                       where("first_name"_c == first_name);
    return query(conn).value();
}
```

Without `AlphaNumeric` validation, this code would be vulnerable to SQL injection during query filtering:

```cpp
// Malicious query parameter that would be rejected by AlphaNumeric
get_people(conn, "Homer' OR '1'='1");  // Attempt to bypass filtering
```

## Deleting data

```cpp
using namespace sqlgen;

const auto query = delete_from<People> |
                   where("first_name"_c == "Hugo");

query(conn).value();
```

This generates the following SQL:

```sql
DELETE FROM "People"
WHERE "first_name" = 'Hugo';
```

## Installation

These three libraries are needed for PostgreSQL support:

```
sudo apt-get install autoconf bison flex
```

To install vcpkg:

```
git submodule update --init
./vcpkg/bootstrap-vcpkg.sh # Linux, macOS
./vcpkg/bootstrap-vcpkg.bat # Windows
# You may be prompted to install additional dependencies.
```

To compile the library:

```
cmake -S . -B build -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4 # gcc, clang
cmake --build build --config Release -j 4 # MSVC
```
