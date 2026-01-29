#pragma once

#include <string>
class RPN
{
  public:
    [[nodiscard]] static int evaluate(const std::string &expression);

  private:
    [[nodiscard]] static int performOperation(int left, int right, char opr);
};