# `sqlgen::write`

The `sqlgen::write` interface provides a type-safe way to write data from C++ containers or ranges to a SQL database. It handles table creation, batch processing, and error handling automatically.

## Usage

### Basic Write

Write a container of objects to a database:

```cpp
const auto people = std::vector<Person>({
    Person{.id = 0, .first_name = "Homer", .last_name = "Simpson", .age = 45},
    Person{.id = 1, .first_name = "Bart", .last_name = "Simpson", .age = 10}
});

// Using with a connection reference
const auto conn = sqlgen::sqlite::connect();
sqlgen::write(conn, people);
```

### With Result<Ref<Connection>>

Handle connection creation and writing in a single chain:

```cpp
sqlgen::sqlite::connect("database.db")
    .and_then([&](auto&& conn) { return sqlgen::write(conn, people); })
    .value();
```

### With Iterators

Write a range of objects using iterators:

```cpp
std::vector<Person> people = /* ... */;
sqlgen::write(conn, people.begin(), people.end());
```

## How It Works

The `write` function performs the following operations in sequence:

1. Creates a table if it doesn't exist (using the object's structure)
2. Prepares an insert statement
3. Writes the data in batches of `SQLGEN_BATCH_SIZE`, which you can set at compile time
4. Handles any errors that occur during the process

## Notes

- The function automatically creates the table, if it doesn't exist
- Data is written in batches for better performance
- The `Result<Nothing>` type provides error handling; use `.value()` to extract the result (will throw an exception if there's an error) or handle errors as needed
- The function has three overloads:
  1. Takes a connection reference and iterators
  2. Takes a `Result<Ref<Connection>>` and iterators
  3. Takes a connection and a container directly

