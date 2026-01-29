#include "RPN.hpp"
#include <cctype>
#include <stack>
#include <stdexcept>

int RPN::evaluate(const std::string &expression)
{
    std::stack<int> stk;
    int result = 0;

    if (expression.empty() || expression.find_first_not_of(" \t\n\r") == std::string::npos)
    {
        throw std::invalid_argument("Empty expression");
    }

    if (expression.find_first_not_of("0123456789 +-*/") != std::string::npos)
    {
        throw std::invalid_argument("Invalid characters in expression");
    }

    for (char token : expression)
    {
        if (isspace(token) != 0)
        {
            continue;
        }
        if (isdigit(token) != 0)
        {
            stk.push(token - '0');
        }
        else
        {
            if (stk.size() < 2)
            {
                throw std::invalid_argument("Insufficient operands");
            }
            int b = stk.top();
            stk.pop();
            int a = stk.top();
            stk.pop();
            result = performOperation(a, b, token);
            stk.push(result);
        }
    }
    if (stk.size() != 1)
    {
        throw std::invalid_argument("The user input has too many values");
    }
    return stk.top();
}

int RPN::performOperation(int left, int right, char opr)
{
    switch (opr)
    {
    case '+':
        return left + right;
    case '-':
        return left - right;
    case '*':
        return left * right;
    case '/':
        if (right == 0)
        {
            throw std::invalid_argument("Division by zero");
        }
        return left / right;
    default:
        throw std::invalid_argument("Invalid operator");
    }
    return 0;
}