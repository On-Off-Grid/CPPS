#include <iostream>
#include <string>
#include "whatever.hpp"

int main()
{
	int a = 2;
	int b = 5;

	std::cout << "before swap: a=" << a << " b=" << b << '\n';
	swap(a, b);
	std::cout << "after swap:  a=" << a << " b=" << b << '\n';
	std::cout << "min/max ints: " << min(a, b) << " / " << max(a, b) << '\n';

	std::string left = "alpha";
	std::string right = "beta";
	std::cout << "min/max strings: " << ::min(left, right) << " / " << ::max(left, right) << '\n';
	std::cout << "equal values return second: " << ::min(b, b) << " / " << ::max(b, b) << '\n';
	return 0;
}
