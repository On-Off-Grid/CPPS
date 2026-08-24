#include <iostream>
#include <string>
#include "iter.hpp"

template <typename T>
void printValue(const T& value)
{
	std::cout << value << ' ';
}

template <typename T>
void incrementValue(T& value)
{
	++value;
}

int main()
{
	int numbers[] = {1, 2, 3, 4, 5};
	const std::size_t numberCount = sizeof(numbers) / sizeof(numbers[0]);
	const int constants[] = {10, 20, 30};
	const std::size_t constantCount = sizeof(constants) / sizeof(constants[0]);
	std::string words[] = {"one", "two", "three"};
	const std::size_t wordCount = sizeof(words) / sizeof(words[0]);

	iter(numbers, numberCount, printValue<int>);
	std::cout << '\n';
	iter(numbers, numberCount, incrementValue<int>);
	iter(numbers, numberCount, printValue<int>);
	std::cout << '\n';
	iter(constants, constantCount, printValue<int>);
	std::cout << '\n';
	iter(words, wordCount, printValue<std::string>);
	std::cout << '\n';
	return 0;
}
