# `sqlgen::read` 

The `sqlgen::read` interface provides a flexible and type-safe way to query data from a SQL database into C++ containers or ranges. It supports composable query building with `where`, `order_by`, and `limit` clauses.

## Usage

### Basic Read

Read all rows from a table into a container (e.g., `std::vector<Person>`):

```cpp
const auto conn = sqlgen::sqlite::connect("database.db");

const auto people = sqlgen::read<std::vector<Person>>(conn).value();
```

Note that `conn` is actually a connection wrapped into an `sqlgen::Result<...>`.
This means you can use monadic error handling and fit this into a single line:

```cpp
const auto people = sqlgen::sqlite::connect("database.db").and_then(
                        sqlgen::read<std::vector<Person>>).value();
```

Please refer to the documentation on `sqlgen::Result<...>` for more information.

### With `where` clause

Filter results using a `where` clause:

```cpp
using namespace sqlgen;

const auto query = sqlgen::read<std::vector<Person>> |
                   where("age"_c < 18 and "first_name"_c != "Hugo");

const auto minors = query(conn).value();
```

Note that `"..."_c` refers to the name of the column. If such a field does not
exists on the struct `Person`, the code will fail to compile.

You can also use monadic error handling here:

```cpp
using namespace sqlgen;

const auto query = sqlgen::read<std::vector<Person>> |
                   where("age"_c < 18 and "first_name"_c != "Hugo");

const auto minors = sqlite::connect("database.db").and_then(query).value();
```

### With `order_by` and `limit`

Sort and limit results:

```cpp
using namespace sqlgen;

const auto query = sqlgen::read<std::vector<Person>> |
                   order_by("age"_c) |
                   limit(2);

const auto youngest_two = query(conn).value();
```

### With ranges

Read results as a lazy range:

```cpp
const auto people_range = sqlgen::read<sqlgen::Range<Person>>(conn).value();
for (const sqlgen::Result<Person>& person : people_range) {
    // process result 
}
```

## Example: Full Query Composition

```cpp
using namespace sqlgen;

const auto query = sqlgen::read<std::vector<Person>> |
                   where("age"_c >= 18) |
                   order_by("last_name"_c, "first_name"_c.desc()) |
                   limit(10);

const auto adults = query(conn).value();
```

It is strongly recommended that you use `using namespace sqlgen`. However,
if you do not want to do that, you can rewrite the example above as follows:

```cpp
const auto query = sqlgen::read<std::vector<Person>> |
                   sqlgen::where(sqlgen::col<"age"> >= 18) |
                   sqlgen::order_by(sqlgen::col<"last_name">, sqlgen::col<"first_name">.desc()) |
                   sqlgen::limit(10);

const auto adults = query(conn).value();
```

## Notes

- All query clauses (`where`, `order_by`, `limit`) are optional.
- The `Result<ContainerType>` type provides error handling; use `.value()` to extract the result (will throw a exception if the results) or handle errors as needed. Refer to the 
- The `sqlgen::Range<T>` type allows for lazy iteration over results.
- `"..."_c` refers to the name of the column.