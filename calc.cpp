#include <string>
#include <stdexcept>
#include <sstream>

#include "calc.h"

void ExtendedExpressionParser::processOperator(std::stack<std::shared_ptr<Expression>>& terms, char op)
{
    if (terms.size() < 2) {
        throw std::runtime_error("Insufficient values in expression");
    }

    auto right = terms.top();
    terms.pop();

    auto left = terms.top();
    terms.pop();

    terms.push(opMap[op](left, right));
}

std::shared_ptr<Expression> ExtendedExpressionParser::parseFraction(std::istringstream& iss) {
    double num, denom;
    char slash;
    iss >> num >> slash >> denom;
    return std::make_shared<Fraction>(num, denom);
}

std::shared_ptr<Expression> ExtendedExpressionParser::parseFunction(std::istringstream& iss, const std::string& funcName) {
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

ExtendedExpressionParser::ExtendedExpressionParser()
{
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

std::shared_ptr<Expression> ExtendedExpressionParser::parse(const std::string& input)
{
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

