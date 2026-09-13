#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <list>

class RPN {
private:
    std::list<double> _stack;

public:
    RPN();
    RPN(const RPN &other);
    RPN &operator=(const RPN &other);
    ~RPN();

    static bool evaluate(const std::string &expression, double &result);
};

#endif
