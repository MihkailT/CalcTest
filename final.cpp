#include <iostream>
#include "calc.h"

int main() {
    // Пример использования
    ExtendedExpressionParser parser;
    try {
        std::cout << "Enter a mathematical expression: ";
        std::string input;
        std::getline(std::cin, input); // 3 + 4 * 2 / (1 - 5)^2 + sin(3.14 / 2) + 1/2 * cos(0)

        auto expr = parser.parse(input);
        std::cout << "Result: " << expr->evaluate() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
