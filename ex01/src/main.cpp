#include "RPN.hpp"

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#define RESET "\033[0m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"

static int t_passed = 0; // NOLINT
static int t_total = 0;  // NOLINT

std::string capture(const std::function<void()> &func)
{
    std::streambuf *orig = std::cout.rdbuf();
    std::ostringstream capture;

    std::cout.rdbuf(capture.rdbuf());
    func();
    std::cout.rdbuf(orig);

    return capture.str();
}

void result(const std::string &msg, bool passed)
{
    t_total++;
    if (passed)
    {
        std::cout << GREEN << "[passed]" << RESET << " " << msg << '\n';
        t_passed++;
        return;
    }
    std::cout << RED << "[failed]" << RESET << " " << msg << '\n';
}

void test_simple_addition()
{
    std::string output = capture([&]() {
        int res = RPN::evaluate("2 3 +");
        std::cout << "Result: " << res << "\n";
    });
    result("2 3 + == 5", output.find("Result: 5") != std::string::npos);
}

void test_simple_subtraction()
{
    std::string output = capture([&]() {
        int res = RPN::evaluate("7 4 -");
        std::cout << "Result: " << res << "\n";
    });
    result("7 4 - == 3", output.find("Result: 3") != std::string::npos);
}

void test_simple_multiplication()
{
    std::string output = capture([&]() {
        int res = RPN::evaluate("2 3 *");
        std::cout << "Result: " << res << "\n";
    });
    result("2 3 * == 6", output.find("Result: 6") != std::string::npos);
}

void test_simple_division()
{
    std::string output = capture([&]() {
        int res = RPN::evaluate("8 2 /");
        std::cout << "Result: " << res << "\n";
    });
    result("8 2 / == 4", output.find("Result: 4") != std::string::npos);
}

void test_complex_expression()
{
    std::string output = capture([&]() {
        int res = RPN::evaluate("1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /");
        std::cout << "Result: " << res << "\n";
    });
    result("complex expression", output.find("Result: 15") != std::string::npos);
}

void test_invalid_token()
{
    bool threw = false;
    try
    {
        (void)RPN::evaluate("2 a +");
    }
    catch (const std::exception &e)
    {
        threw = true;
    }
    result("invalid token error", threw);
}

void test_division_by_zero()
{
    bool threw = false;
    try
    {
        (void)RPN::evaluate("5 0 /");
    }
    catch (const std::exception &e)
    {
        threw = true;
    }
    result("division by zero error", threw);
}

void test_too_few_operands()
{
    bool threw = false;
    try
    {
        (void)RPN::evaluate("+");
    }
    catch (const std::exception &e)
    {
        threw = true;
    }
    result("too few operands error", threw);
}

void test_too_many_operands()
{
    bool threw = false;
    try
    {
        (void)RPN::evaluate("1 2 3 +");
    }
    catch (const std::exception &e)
    {
        threw = true;
    }
    result("too many operands error", threw);
}

void test_operand_greater_than_9()
{
    bool threw = false;
    try
    {
        (void)RPN::evaluate("10 2 +");
    }
    catch (const std::exception &e)
    {
        threw = true;
    }
    result("operand > 9 error", threw);
}

int main(int argc, char **argv)
{
    // if (argc == 1)
    // {
    //     test_simple_addition();
    //     test_simple_subtraction();
    //     test_simple_multiplication();
    //     test_simple_division();
    //     test_complex_expression();
    //     test_invalid_token();
    //     test_division_by_zero();
    //     test_too_few_operands();
    //     test_too_many_operands();
    //     test_operand_greater_than_9();
    //     std::cout << "\n" << t_passed << "/" << t_total << " tests passed.\n";
    //     return t_passed == t_total ? 0 : 1;
    // }
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <notation>\n";
        return 1;
    }

    try
    {
        std::cout << "Result: " << RPN::evaluate(argv[1]) << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}