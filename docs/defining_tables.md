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

## Primary keys

Of course, you would also like to set primary keys on your
tables. This is possible as well:

```cpp
struct People {
    rfl::PrimaryKey<std::string> first_name;
    std::string last_name;
    uint age;
};
```

Now, the generated SQL code will look something like this:

```sql
CREATE TABLE IF NOT EXISTS "People"("first_name" TEXT NOT NULL, 
                                    "last_name"  TEXT NOT NULL, 
                                    "age"        INTEGER NOT NULL,
                                    PRIMARY_KEY("first_name"));
```

`rfl::PrimaryKey<...>` is a simple wrapper. You can simply assign to it as follows:

```cpp
const auto person = People{.first_name = "Homer", ...};
```

And you can retrieve the underlying value using any of the following:

```cpp
person.first_name();
person.first_name.get();
person.first_name.value();
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
