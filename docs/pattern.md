# `sqlgen::Pattern`

The `sqlgen::Pattern` class provides a type-safe way to validate string fields against regular expressions in C++. It's particularly useful for preventing SQL injection by ensuring data conforms to expected patterns before it reaches the database.

## Usage

### Basic Definition

Define a pattern using a regular expression and a descriptive name:

```cpp
// Define a pattern for usernames (alphanumeric with underscores)
using Username = sqlgen::Pattern<"[a-zA-Z0-9_]+", "username">;

struct User {
    sqlgen::PrimaryKey<uint32_t> id;
    Username username;
    sqlgen::Email email;
};
```

### Built-in Patterns

SQLGen provides several commonly used patterns out of the box:

```cpp
// Alphanumeric strings
sqlgen::AlphaNumeric name;

// Base64 encoded strings
sqlgen::Base64Encoded encoded_data;

// Email addresses
sqlgen::Email email;

// UUID formats
sqlgen::UUIDv1 uuid1;
sqlgen::UUIDv2 uuid2;
sqlgen::UUIDv3 uuid3;
sqlgen::UUIDv4 uuid4;
```

### Assignment and Validation

Assign values to pattern-validated fields:

```cpp
struct Person {
    sqlgen::PrimaryKey<uint32_t> id;
    sqlgen::AlphaNumeric first_name;
    sqlgen::AlphaNumeric last_name;
    int age;
};

const auto person = Person{
    .id = 1,
    .first_name = "Homer",  // Valid: contains only alphanumeric characters
    .last_name = "Simpson", // Valid: contains only alphanumeric characters
    .age = 45
};
```

### Accessing Values

Access the underlying string value:

```cpp
const auto person = Person{
    .first_name = "Homer",
    .last_name = "Simpson"
};

// Get the value
const std::string& value1 = person.first_name();
const std::string& value2 = person.first_name.get();
const std::string& value3 = person.first_name.value();
```

## SQL Injection Prevention

Pattern validation is a crucial security feature that helps prevent SQL injection attacks by:

1. Enforcing strict input validation before data reaches the database
2. Ensuring data conforms to expected formats
3. Rejecting potentially malicious input that doesn't match the pattern

For example, using `AlphaNumeric` for usernames prevents injection of SQL commands:

```cpp
struct User {
    sqlgen::PrimaryKey<uint32_t> id;
    sqlgen::AlphaNumeric username;  // Rejects: "admin'; DROP TABLE users;--"
    sqlgen::Email email;           // Rejects: "invalid@email'; DELETE FROM users;--"
};
```

## Notes

- Pattern validation occurs at runtime when values are assigned
- The template parameters are:
  - `_regex`: The regular expression pattern to match against
  - `_name`: A descriptive name for the pattern (used in error messages)
- The class supports:
  - Direct string assignment with validation
  - Multiple access methods for the underlying value
  - Reflection for SQL operations
  - Move and copy semantics
- Built-in patterns provide common validation rules
- Custom patterns can be defined for specific use cases
- Pattern validation is an important part of a defense-in-depth security strategy
