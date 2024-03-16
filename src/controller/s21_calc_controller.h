#ifndef SRC_S21_CALC_CONTROLLER_H
#define SRC_S21_CALC_CONTROLLER_H

#include <string>

#include "./../model/s21_calc_model.h"

namespace s21 {
class CalculatorController {
 public:
  CalculatorController() = default;
  ~CalculatorController() = default;
  // inline
  void SetExpersion(const std::string &infix, const long double x) noexcept {
    expression_.SetInfix(infix);
    expression_.SetX(x);
  }

  void Calculate() { expression_.eval_expression(); }

  //        inline
  long double GetResult() const noexcept { return expression_.GetResult(); }

  //        inline
  enum Status GetStatus() const noexcept { return expression_.GetStatus(); }

 private:
  CalcModel expression_;

};  // CalculatorController

}  // namespace s21

#endif  // SRC_S21_CALC_CONTROLLER_H
