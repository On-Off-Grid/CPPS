#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Array.hpp"

#define MAX_VAL 750

int main(int, char**)
{
	Array<int> numbers(MAX_VAL);
	int* mirror = new int[MAX_VAL];

	std::srand(static_cast<unsigned int>(std::time(0)));
	for (int index = 0; index < MAX_VAL; ++index)
	{
		const int value = std::rand();
		numbers[index] = value;
		mirror[index] = value;
	}
	{
		Array<int> tmp = numbers;
		Array<int> test(tmp);
		(void)test;
	}
	for (int index = 0; index < MAX_VAL; ++index)
	{
		if (mirror[index] != numbers[index])
		{
			std::cerr << "didn't save the same value!!" << std::endl;
			delete[] mirror;
			return 1;
		}
	}
	try
	{
		numbers[-2] = 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		numbers[MAX_VAL] = 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	for (int index = 0; index < MAX_VAL; ++index)
		numbers[index] = std::rand();
	delete[] mirror;
	return 0;
}
