# `sqlgen::Result`

The `sqlgen::Result` type provides a way to handle errors without exceptions in sqlgen. It is an alias for `rfl::Result` and is used throughout sqlgen's API to handle potential failures in database operations.

Note that `sqlgen::Result` and `rfl::Result` are deliberately designed
to resemble C++ 23's `std::expected`.

## Usage

### Basic Usage

Handle database operations that might fail:

```cpp
// Reading from database
const auto people = sqlgen::read<std::vector<Person>>(conn);

if (people) {
    // Success case - use the value
    const auto& result = *people;
} else {
    // Error case - handle the error
    const auto& error = people.error();
}

// Writing to database
const auto write_result = sqlgen::write(conn, data);

if (!write_result) {
    // Handle write error
    const auto& error = write_result.error();
}
```

### Safe Value Access

Access values safely using different methods:

```cpp
const auto result = sqlgen::read<std::vector<Person>>(conn);

// Method 1: Using .value() (throws if error)
const auto person = result.value();

// Method 2: Using .value_or() (provides default if error)
const auto person = result.value_or(std::vector<Person>());

// Method 3: Using operator * (undefined if error)
if (result) {
    const auto& person = *result;
}
```

### Monadic Operations

Chain operations using monadic functions:

```cpp
// Transform successful results - note that this
// is not a particularly efficient way of counting, 
// but it highlights the point.
const auto count = sqlgen::read<std::vector<Person>>(conn)
    .transform([](const auto& people) {
        return people.size();
    });

// Chain operations that might fail
const auto result = sqlgen::sqlite::connect("database.db")
    .and_then(sqlgen::read<std::vector<Person>>);
```

### Error Handling

Handle errors explicitly:

```cpp
const auto result = sqlgen::read<std::vector<Person>>(conn);

// Check for errors
if (!result) {
    const auto& error = result.error();
    // Handle error...
}

// Transform errors
const auto better_error = result.transform_error([](const auto& e) {
    return sqlgen::error("Database error: " + e.what());
});
```

## Notes

- `sqlgen::Result` is an alias for `rfl::Result`
- All database operations in sqlgen return `Result` types
- The type supports:
  - Safe value access with `.value()` and `.value_or()`
  - Monadic operations with `.transform()` and `.and_then()`
  - Error handling with `.error()`
  - Boolean checks for success/failure
  - Move and copy semantics
- It's recommended to handle errors explicitly rather than using `.value()` which throws exceptions
