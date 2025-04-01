# sqlgen

sqlgen is an ORM and SQL query generator for C++-20, similar to Python's [SQLAlchemy](https://github.com/sqlalchemy/sqlalchemy)/[SQLModel](https://github.com/fastapi/sqlmodel) or Rust's [Diesel](https://github.com/diesel-rs/diesel).

Much like SQLModel is closely integrated with [pydantic](https://github.com/pydantic/pydantic),
sqlgen is closely integrated with our sister project [reflect-cpp](https://github.com/getml/reflect-cpp). This allows you to construct very reliable and highly efficient data pipelines.

## Simple example

Here is how you create a simple sqlite database
and insert some data:

```cpp
#include <sqlgen/sqlite.hpp>

struct People {
    std::string first_name;
    std::string last_name;
    uint age;
};

const auto people = std::vector<People>({
    person{.first_name = "Homer",
           .last_name = "Simpson",
           .age = 45}});

const auto conn = sqlgen::sqlite::connect("example.db");

// Will automatically create a table called 'People'
// with the columns 'first_name', 'last_name' and 'age', 
// if necessary.
const auto result = sqlgen::write(conn, people);

if (!result) {
    std::cout << result.error().what() << std::endl;
}
```

Here is how you retrieve the same data from the database
and print the results as a JSON:

```cpp
#include <rfl/json.hpp> // reflect-cpp
#include <sqlgen/sqlite.hpp>

const auto conn = sqlgen::sqlite::connect("example.db");

const sqlgen::Result<std::vector<People>> result = 
    sqlgen::read<std::vector<People>>(conn);

if (result) {
    std::cout << rfl::json::write(*result) << std::endl;
} else {
    std::cout << result.error().what() << std::endl;
}
```

## Installation

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
