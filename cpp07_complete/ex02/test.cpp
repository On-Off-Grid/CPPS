#include "Array.hpp"
#include <iostream>

int main()
{
  Array<std::string> strArr(3);
  strArr[0] = "hello";
  strArr[1] = "world";
  strArr[2] = "!";

  std::cout << strArr[0] << " " << strArr[1] << " " << strArr[2] << std::endl;

  Array<int> empty;
  std::cout << empty.size() << std::endl;

  Array<int> specific(10);
  for (std::size_t i = 0; i < specific.size(); ++i)
    specific[i] = i;
  specific[0] = 5;
  specific[1] = 6;

  for (std::size_t i = 0; i < specific.size(); ++i)
    std::cout << "specific[ " << i << "] = " << specific[i] << std::endl;
  
} 
