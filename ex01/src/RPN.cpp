#include "RPN.hpp"
#include <cctype>
#include <stack>
#include <stdexcept>

RPN::RPN()
{
}

RPN::~RPN()
{
}

RPN::RPN(const RPN &other)
{
    (void)other;
}

RPN &RPN::operator=(const RPN &other)
{
    (void)other;
    return *this;
}

int RPN::evaluate(const std::string &expression) const
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

    for (auto it = expression.begin(); it != expression.end(); ++it)
    {
        char token = *it;

        if (isspace(token))
        {
            continue;
        }
        if (isdigit(token))
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

int RPN::performOperation(int left, int right, char opr) const
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