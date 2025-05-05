#ifndef SQLGEN_TRANSPILATION_CONDITIONS_HPP_
#define SQLGEN_TRANSPILATION_CONDITIONS_HPP_

namespace sqlgen::transpilation::conditions {

template <class CondType1, class CondType2>
struct And {
  CondType1 cond1;
  CondType2 cond2;
};

template <class OpType1, class OpType2>
struct Equal {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
auto equal(const OpType1& _op1, const OpType2& _op2) {
  return Equal<std::remove_cvref_t<OpType1>, std::remove_cvref_t<OpType2>>{
      .op1 = _op1, .op2 = _op2};
}

template <class OpType1, class OpType2>
struct GreaterEqual {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
auto greater_equal(const OpType1& _op1, const OpType2& _op2) {
  return GreaterEqual<std::remove_cvref_t<OpType1>,
                      std::remove_cvref_t<OpType2>>{.op1 = _op1, .op2 = _op2};
}

template <class OpType1, class OpType2>
struct GreaterThan {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
auto greater_than(const OpType1& _op1, const OpType2& _op2) {
  return GreaterThan<std::remove_cvref_t<OpType1>,
                     std::remove_cvref_t<OpType2>>{.op1 = _op1, .op2 = _op2};
}

template <class OpType1, class OpType2>
struct NotEqual {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
auto not_equal(const OpType1& _op1, const OpType2& _op2) {
  return NotEqual<std::remove_cvref_t<OpType1>, std::remove_cvref_t<OpType2>>{
      .op1 = _op1, .op2 = _op2};
}

template <class OpType1, class OpType2>
struct LesserEqual {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
auto lesser_equal(const OpType1& _op1, const OpType2& _op2) {
  return LesserEqual<std::remove_cvref_t<OpType1>,
                     std::remove_cvref_t<OpType2>>{.op1 = _op1, .op2 = _op2};
}

template <class OpType1, class OpType2>
struct LesserThan {
  OpType1 op1;
  OpType2 op2;
};

template <class OpType1, class OpType2>
auto lesser_than(const OpType1& _op1, const OpType2& _op2) {
  return LesserThan<std::remove_cvref_t<OpType1>, std::remove_cvref_t<OpType2>>{
      .op1 = _op1, .op2 = _op2};
}

template <class CondType1, class CondType2>
struct Or {
  CondType1 cond1;
  CondType2 cond2;
};

}  // namespace sqlgen::transpilation::conditions

#endif
