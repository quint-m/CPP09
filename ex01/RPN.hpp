#pragma once

#include <string>
class RPN
{
  public:
    RPN();
    ~RPN();
    RPN(const RPN &other);
    RPN &operator=(const RPN &other);

    [[nodiscard]] int evaluate(const std::string &expression) const;

  private:
    [[nodiscard]] int performOperation(int left, int right, char opr) const;
};