#include <iostream>
#include <vector>
#include <list>
#include "easyfind.hpp"

int main() {
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);

    try {
        std::cout << "Testing easyfind with std::vector:\n";
        std::vector<int>::iterator it = easyfind(vec, 3);
        std::cout << "Found value: " << *it << "\n";

        it = easyfind(vec, 10);
        std::cout << "Found value: " << *it << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

    std::list<int> lst;
    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);

    try {
        std::cout << "\nTesting easyfind with std::list:\n";
        std::list<int>::iterator it = easyfind(lst, 20);
        std::cout << "Found value: " << *it << "\n";

        it = easyfind(lst, 100);
        std::cout << "Found value: " << *it << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

    return 0;
}
