# `sqlgen::PrimaryKey` 

Sometimes you would like to set primary keys on your
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

