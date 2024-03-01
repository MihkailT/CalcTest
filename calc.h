#ifndef FINAL_H
#define FINAL_H

#include <cmath>
#include <vector>
#include <stack>
#include <cctype>
#include <memory>
#include <map>
#include <functional>

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

    void processOperator(std::stack<std::shared_ptr<Expression>>& terms, char op);

    std::shared_ptr<Expression> parseFraction(std::istringstream& iss);

    std::shared_ptr<Expression> parseFunction(std::istringstream& iss, const std::string& funcName);

public:
    ExtendedExpressionParser();

    std::shared_ptr<Expression> parse(const std::string& input);
};

#endif // FINAL_H
