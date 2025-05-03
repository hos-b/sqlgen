#ifndef SQLGEN_TRANSPILATION_CONDITIONS_HPP_
#define SQLGEN_TRANSPILATION_CONDITIONS_HPP_

namespace sqlgen::transpilation::conditions {

template <class CondType1, class CondType2>
struct And {
  CondType1 lhs;
  CondType2 rhs;
};

template <class OpType1, class OpType2>
struct Equals {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
struct GreaterEqual {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
struct GreaterThan {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
struct NotEquals {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
struct LesserEqual {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
struct LesserThan {
  OpType1 op1;
  OpType2 op2;
};

template <class CondType1, class CondType2>
struct Or {
  CondType1 lhs;
  CondType2 rhs;
};

}  // namespace sqlgen::transpilation::conditions

#endif
