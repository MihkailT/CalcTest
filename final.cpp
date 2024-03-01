#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <map>
#include <functional>
#include <stack>
#include <cctype>

// Включение необходимых заголовочных файлов для стандартных библиотек C++

// Абстрактный класс для выражений
class Expression {
public:
    virtual ~Expression() = default;
    virtual double evaluate() const = 0;
};

// Определение абстрактного класса Expression, который представляет выражения и содержит чисто виртуальную функцию evaluate

// Класс для представления чисел
class Number : public Expression {
    double value;

public:
    Number(double val) : value(val) {}

    double evaluate() const override {
        return value;
    }
};

// Класс для представления чисел и их вычисления

// Базовый класс для бинарных операций
class BinaryOperation : public Expression {
protected:
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;

public:
    BinaryOperation(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
        : left(l), right(r) {}
};

// Определение базового класса BinaryOperation для бинарных операций

// Классы для арифметических операций
class Add : public BinaryOperation {
public:
    using BinaryOperation::BinaryOperation;

    double evaluate() const override {
        return left->evaluate() + right->evaluate();
    }
};

// Класс для операции сложения

class Subtract : public BinaryOperation {
public:
    using BinaryOperation::BinaryOperation;

    double evaluate() const override {
        return left->evaluate() - right->evaluate();
    }
};

// Класс для операции вычитания

class Multiply : public BinaryOperation {
public:
    using BinaryOperation::BinaryOperation;

    double evaluate() const override {
        return left->evaluate() * right->evaluate();
    }
};

// Класс для операции умножения

class Divide : public BinaryOperation {
public:
    using BinaryOperation::BinaryOperation;

    double evaluate() const override {
        double rightEval = right->evaluate();
        if (rightEval == 0) {
            throw std::runtime_error("Division by zero");
        }
        return left->evaluate() / rightEval;
    }
};

// Класс для операции деления, с проверкой деления на ноль

class Power : public BinaryOperation {
public:
    using BinaryOperation::BinaryOperation;

    double evaluate() const override {
        return std::pow(left->evaluate(), right->evaluate());
    }
};

// Класс для операции возведения в степень

class Fraction : public Expression {
    double numerator;
    double denominator;

public:
    Fraction(double num, double denom) : numerator(num), denominator(denom) {
        if (denom == 0) {
            throw std::runtime_error("Denominator cannot be zero");
        }
    }

    double evaluate() const override {
        return numerator / denominator;
    }
};

// Класс для представления дробей, с проверкой деления на ноль

class Sin : public Expression {
    std::shared_ptr<Expression> operand;

public:
    Sin(std::shared_ptr<Expression> op) : operand(op) {}

    double evaluate() const override {
        return std::sin(operand->evaluate());
    }
};

// Класс для операции синуса

class Cos : public Expression {
    std::shared_ptr<Expression> operand;

public:
    Cos(std::shared_ptr<Expression> op) : operand(op) {}

    double evaluate() const override {
        return std::cos(operand->evaluate());
    }
};

// Класс для операции косинуса

// Класс для парсинга и обработки входных данных
class ExtendedExpressionParser {
    std::map<char, int> opPrecedence;
    std::map<char, std::function<std::shared_ptr<Expression>(std::shared_ptr<Expression>, std::shared_ptr<Expression>)>> opMap;

    // Класс для парсинга и обработки входных данных
    bool isOperator(char c) {
        return opMap.find(c) != opMap.end();
    }

    int getPrecedence(char op) {
        return opPrecedence[op];
    }

    bool isHigherPrecedence(char op1, char op2) {
        return getPrecedence(op1) > getPrecedence(op2);
    }

    void processOperator(std::stack<std::shared_ptr<Expression>>& terms, char op) {
        if (terms.size() < 2) {
            throw std::runtime_error("Insufficient values in expression");
        }

        auto right = terms.top();
        terms.pop();

        auto left = terms.top();
        terms.pop();

        terms.push(opMap[op](left, right));
    }

    std::shared_ptr<Expression> parseFraction(std::istringstream& iss) {
        double num, denom;
        char slash;
        iss >> num >> slash >> denom;
        return std::make_shared<Fraction>(num, denom);
    }

    std::shared_ptr<Expression> parseFunction(std::istringstream& iss, const std::string& funcName) {
        std::string argStr;
        char next;
        int bracketCount = 1;

        // Пропустить следующую скобку '('
        iss >> next;
        if (next != '(') {
            throw std::runtime_error("Expected '(' after function name");
        }

        // Извлекаем аргумент функции до закрывающей скобки ')'
        while (iss >> next) {
            if (next == '(') {
                bracketCount++;
            }
            else if (next == ')') {
                bracketCount--;
                if (bracketCount == 0) break;
            }
            argStr += next;
        }

        if (bracketCount != 0) {
            throw std::runtime_error("Mismatched parentheses in function argument");
        }

        auto arg = parse(argStr);  // Парсим подстроку

        if (funcName == "sin") {
            return std::make_shared<Sin>(arg);
        }
        else if (funcName == "cos") {
            return std::make_shared<Cos>(arg);
        }
        else {
            throw std::runtime_error("Unknown function: " + funcName);
        }
    }

public:
    ExtendedExpressionParser() {
        // Определение приоритетов операторов
        opPrecedence['+'] = 1;
        opPrecedence['-'] = 1;
        opPrecedence['*'] = 2;
        opPrecedence['/'] = 2;
        opPrecedence['^'] = 3;

        // Определение действий для операторов
        opMap['+'] = [](std::shared_ptr<Expression> a, std::shared_ptr<Expression> b) { return std::make_shared<Add>(a, b); };
        opMap['-'] = [](std::shared_ptr<Expression> a, std::shared_ptr<Expression> b) { return std::make_shared<Subtract>(a, b); };
        opMap['*'] = [](std::shared_ptr<Expression> a, std::shared_ptr<Expression> b) { return std::make_shared<Multiply>(a, b); };
        opMap['/'] = [](std::shared_ptr<Expression> a, std::shared_ptr<Expression> b) { return std::make_shared<Divide>(a, b); };
        opMap['^'] = [](std::shared_ptr<Expression> a, std::shared_ptr<Expression> b) { return std::make_shared<Power>(a, b); };
    }

    std::shared_ptr<Expression> parse(const std::string& input) {
        std::istringstream iss(input);
        std::stack<std::shared_ptr<Expression>> terms;
        std::stack<char> ops;
        char c;

        while (iss >> c) {
            if (isdigit(c) || c == '.') {
                iss.putback(c);
                double num;
                iss >> num;
                terms.push(std::make_shared<Number>(num));
            }
            else if (isOperator(c)) {
                while (!ops.empty() && isOperator(ops.top()) && isHigherPrecedence(ops.top(), c)) {
                    processOperator(terms, ops.top());
                    ops.pop();
                }
                ops.push(c);
            }
            else if (c == '(') {
                ops.push(c);
            }
            else if (c == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    processOperator(terms, ops.top());
                    ops.pop();
                }
                if (!ops.empty() && ops.top() == '(') {
                    ops.pop();
                }
                else {
                    throw std::runtime_error("Mismatched parentheses");
                }
            }
            else if (isalpha(c)) {
                std::string func;
                func += c;
                while (iss.peek() != '(' && iss >> c) {
                    func += c;
                }
                if (func == "sin" || func == "cos") {
                    terms.push(parseFunction(iss, func));
                }
                else {
                    throw std::runtime_error("Unknown function: " + func);
                }
            }
            else {
                throw std::runtime_error("Invalid character or token");
            }
        }

        while (!ops.empty()) {
            processOperator(terms, ops.top());
            ops.pop();
        }

        if (terms.size() != 1) {
            throw std::runtime_error("Invalid expression");
        }

        return terms.top();
    }
};

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