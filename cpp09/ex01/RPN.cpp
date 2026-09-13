#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <cctype>

RPN::RPN() {}

RPN::RPN(const RPN &other) : _stack(other._stack) {}

RPN &RPN::operator=(const RPN &other) {
    if (this != &other) {
        _stack = other._stack;
    }
    return *this;
}

RPN::~RPN() {}

bool RPN::evaluate(const std::string &expression, double &result) {
    std::list<double> stack;
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token) {
        if (token.length() == 1 && std::isdigit(token[0])) {
            stack.push_back(token[0] - '0');
        } else if (token.length() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/')) {
            if (stack.size() < 2)
                return false;

            double rhs = stack.back();
            stack.pop_back();
            double lhs = stack.back();
            stack.pop_back();

            if (token[0] == '+') {
                stack.push_back(lhs + rhs);
            } else if (token[0] == '-') {
                stack.push_back(lhs - rhs);
            } else if (token[0] == '*') {
                stack.push_back(lhs * rhs);
            } else if (token[0] == '/') {
                if (rhs == 0)
                    return false;
                stack.push_back(lhs / rhs);
            }
        } else {
            return false;
        }
    }

    if (stack.size() != 1)
        return false;

    result = stack.back();
    return true;
}
