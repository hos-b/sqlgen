# `sqlgen::PrimaryKey`

`sqlgen::PrimaryKey` is used to indicate which key should be a primary key.

## Usage

### Basic Definition

Define a primary key field in your struct:

```cpp
struct People {
    sqlgen::PrimaryKey<std::string> first_name;
    std::string last_name;
    uint age;
};
```

This generates the following SQL schema:

```sql
CREATE TABLE IF NOT EXISTS "People"(
    "first_name" TEXT NOT NULL, 
    "last_name"  TEXT NOT NULL, 
    "age"        INTEGER NOT NULL,
    PRIMARY_KEY("first_name")
);
```

### Assignment and Access

Assign values to primary key fields:

```cpp
const auto person = People{
    .first_name = "Homer",
    .last_name = "Simpson",
    .age = 45
};
```

Access the underlying value using any of these methods:

```cpp
person.first_name();
person.first_name.get();
person.first_name.value();
```

## Notes

- The template parameter specifies the type of the primary key field
- Primary key fields are automatically marked as NOT NULL in the generated SQL
- The class supports:
  - Direct value assignment
  - Multiple access methods for the underlying value
  - Reflection for SQL operations
  - Move and copy semantics
- Primary keys can be used with any supported SQL data type

