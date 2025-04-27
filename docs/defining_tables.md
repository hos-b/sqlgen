# Defining tables

In sqlgen, tables are defined using C++ structs. In its simplest 
form, this can look like this:

```cpp
struct People {
    std::string first_name;
    std::string last_name;
    uint age;
};
```

When you then try to write a vector of `People`, 
the table will be automatically created, if necessary:

```cpp
const auto people = std::vector<People>(...);

const auto result = sqlgen::write(conn, people);
```

In other words, `sqlgen::write(...)` might call the following SQL
statement under-the-hood:

```sql
CREATE TABLE IF NOT EXISTS "People"("first_name" TEXT NOT NULL, 
                                    "last_name"  TEXT NOT NULL, 
                                    "age"        INTEGER NOT NULL);

```

(As with all SQL examples, the exact code will vary between dialects.)

## Custom table names

Obviously, it is very convenient to just infer the table name
from the name of the struct. But sometimes we don't want that. 
So we can do something like this:

```cpp
struct People {
    constexpr static const char* tablename = "PEOPLE";

    std::string first_name;
    std::string last_name;
    uint age;
};
```

Now, the generated SQL code will look like this:

```cpp
CREATE TABLE IF NOT EXISTS "PEOPLE"("first_name" TEXT NOT NULL, 
                                    "last_name"  TEXT NOT NULL, 
                                    "age"        INTEGER NOT NULL);
```

## Custom schemata

In a similar manner, you can embed your table in a schema:

```cpp
struct People {
    constexpr static const char* schema = "my_schema";

    std::string first_name;
    std::string last_name;
    uint age;
};
```

Now, the generated SQL code will look like this:

```cpp
CREATE TABLE IF NOT EXISTS "my_schema"."People"("first_name" TEXT NOT NULL, 
                                                "last_name"  TEXT NOT NULL, 
                                                "age"        INTEGER NOT NULL);
```

## Nullable types

As we have seen, all columns are non-nullable by default. But what if we do want
nullability?

As with reflect-cpp, `std::optional`, `std::shared_ptr` and `std::unique_ptr` are interpreted
as nullable types. So you can create nullable fields as follows:

```cpp
struct People {
    rfl::PrimaryKey<std::string> first_name;
    std::string last_name;
    std::optional<uint> age;
};
```

You can induce the NULL value by passing `std::nullopt` to age and NULL values
will be translated to `std::nullopt`.
