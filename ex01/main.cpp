#include "RPN.hpp"
#include <exception>
#include <iostream>
int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <notation>\n";
        return 1;
    }

    RPN rpn;
    try {
        int result = rpn.evaluate(argv[1]);
        std::cout << "Result: " << result << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}