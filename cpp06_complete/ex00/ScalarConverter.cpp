#include "ScalarConverter.hpp"
#include <iostream>

#include <cstdlib>
#include <cerrno>
#include <climits>
#include <cmath>
#include <iomanip>
#include <cctype>

enum PseudoType { P_NONE, P_NAN, P_INF };

static PseudoType detectPseudoLiteral(const std::string &s, bool &hasF, bool &isNeg)
{
    std::string t;
    t.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) t.push_back(std::tolower(s[i]));
    hasF = false;
    isNeg = false;
    if (t.size() == 0) return P_NONE;
    if (!t.empty() && t[t.size() - 1] == 'f') { hasF = true; t.resize(t.size() - 1); }
    if (t.size() > 0 && t[0] == '+') t = t.substr(1);
    if (t.size() > 0 && t[0] == '-') { isNeg = true; t = t.substr(1); }
    if (t == "nan") return P_NAN;
    if (t == "inf" || t == "infinity") return P_INF;
    return P_NONE;
}

void printChar(double v)
{
    if (std::isnan(v) || std::isinf(v))
    {
        std::cout << "char: impossible" << std::endl;
        return;
    }
    long lv = static_cast<long>(v);
    if (lv < 0 || lv > 127)
    {
        std::cout << "char: impossible" << std::endl;
        return;
    }
    char c = static_cast<char>(lv);
    if (!std::isprint(static_cast<unsigned char>(c)))
        std::cout << "char: Non displayable" << std::endl;
    else
        std::cout << "char: '" << c << "'" << std::endl;
}

void printInt(double v)
{
    if (std::isnan(v) || std::isinf(v) || v < static_cast<double>(INT_MIN) || v > static_cast<double>(INT_MAX))
    {
        std::cout << "int: impossible" << std::endl;
        return;
    }
    int iv = static_cast<int>(v);
    std::cout << "int: " << iv << std::endl;
}

void printFloat(double v, PseudoType ptype, bool isNeg)
{
    std::cout << std::fixed << std::setprecision(1);
    if (ptype == P_NAN)
    {
        std::cout << "float: nanf" << std::endl;
        return;
    }
    if (ptype == P_INF)
    {
        std::cout << (isNeg ? "float: -inff" : "float: +inff") << std::endl;
        return;
    }
    if (std::isnan(v)) { std::cout << "float: nanf" << std::endl; return; }
    if (std::isinf(v)) { std::cout << (std::signbit(v) ? "float: -inff" : "float: +inff") << std::endl; return; }
    float fv = static_cast<float>(v);
    std::cout << "float: " << fv << "f" << std::endl;
}

void printDouble(double v, PseudoType ptype, bool isNeg)
{
    std::cout << std::fixed << std::setprecision(1);
    if (ptype == P_NAN)
    {
        std::cout << "double: nan" << std::endl;
        return;
    }
    if (ptype == P_INF)
    {
        std::cout << (isNeg ? "double: -inf" : "double: +inf") << std::endl;
        return;
    }
    if (std::isnan(v)) { std::cout << "double: nan" << std::endl; return; }
    if (std::isinf(v)) { std::cout << (std::signbit(v) ? "double: -inf" : "double: +inf") << std::endl; return; }
    std::cout << "double: " << v << std::endl;
}

void ScalarConverter::convert(const std::string &literal)
{
    const char *s = literal.c_str();

    // char literal (single non-digit printable)
    if (literal.size() == 1 && !std::isdigit(literal[0]))
    {
        double v = static_cast<double>(literal[0]);
        printChar(v);
        printInt(v);
        printFloat(v, P_NONE, false);
        printDouble(v, P_NONE, false);
        return;
    }

    // pseudo-literals
    bool hasF = false;
    bool isNeg = false;
    PseudoType ptype = detectPseudoLiteral(literal, hasF, isNeg);
    if (ptype != P_NONE)
    {
        double v = std::strtod(s, NULL);
        printChar(v);
        printInt(v);
        printFloat(v, ptype, isNeg);
        printDouble(v, ptype, isNeg);
        return;
    }

    // try integer
    errno = 0;
    char *end = NULL;
    long li = std::strtol(s, &end, 10);
    if (end != s && *end == '\0' && errno != ERANGE)
    {
        double v = static_cast<double>(li);
        printChar(v);
        printInt(v);
        printFloat(v, P_NONE, false);
        printDouble(v, P_NONE, false);
        return;
    }

    // try float with trailing 'f'
    size_t len = literal.size();
    if (len > 1 && literal[len - 1] == 'f')
    {
        std::string core = literal.substr(0, len - 1);
        errno = 0;
        char *end2 = NULL;
        double dv = std::strtod(core.c_str(), &end2);
        if (end2 != core.c_str() && *end2 == '\0' && errno != ERANGE)
        {
            printChar(dv);
            printInt(dv);
            printFloat(dv, P_NONE, false);
            printDouble(dv, P_NONE, false);
            return;
        }
    }

    // try double
    errno = 0;
    end = NULL;
    double dd = std::strtod(s, &end);
    if (end != s && *end == '\0' && errno != ERANGE)
    {
        printChar(dd);
        printInt(dd);
        printFloat(dd, P_NONE, false);
        printDouble(dd, P_NONE, false);
        return;
    }

    std::cout << "char: impossible" << std::endl;
    std::cout << "int: impossible" << std::endl;
    std::cout << "float: impossible" << std::endl;
    std::cout << "double: impossible" << std::endl;
}

