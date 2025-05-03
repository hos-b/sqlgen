#ifndef SQLGEN_DYNAMIC_CONDITION_HPP_
#define SQLGEN_DYNAMIC_CONDITION_HPP_

#include <rfl.hpp>

#include "../Ref.hpp"
#include "Column.hpp"
#include "ColumnOrValue.hpp"

namespace sqlgen::dynamic {

struct Condition {
  struct And {
    Ref<Condition> cond1;
    Ref<Condition> cond2;
  };

  struct Equals {
    Column op1;
    ColumnOrValue op2;
  };

  struct GreaterEqual {
    Column op1;
    ColumnOrValue op2;
  };

  struct GreaterThan {
    Column op1;
    ColumnOrValue op2;
  };

  struct NotEquals {
    Column op1;
    ColumnOrValue op2;
  };

  struct LesserEqual {
    Column op1;
    ColumnOrValue op2;
  };

  struct LesserThan {
    Column op1;
    ColumnOrValue op2;
  };

  struct Or {
    Ref<Condition> cond1;
    Ref<Condition> cond2;
  };

  using ReflectionType =
      rfl::TaggedUnion<"what", And, Equals, GreaterEqual, GreaterThan,
                       NotEquals, LesserEqual, LesserThan, Or>;

  const ReflectionType& reflection() const { return val; }

  ReflectionType val;
};

}  // namespace sqlgen::dynamic

#endif
