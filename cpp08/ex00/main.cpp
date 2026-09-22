#include <iostream>
#include <vector>
#include <list>
#include "easyfind.hpp"

int main() {
    // std::vector: sequence container providing dynamic contiguous array storage
    std::vector<int> vec;
    // push_back(): vector container method that appends an element to the end of the container
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);

    try {
        std::cout << "Testing easyfind with std::vector:\n";
        // Container iterator type (std::vector<int>::iterator) used to store the result of easyfind
        std::vector<int>::iterator it = easyfind(vec, 3);
        // *it: dereferencing the iterator to access the element value
        std::cout << "Found value: " << *it << "\n";

        it = easyfind(vec, 10);
        std::cout << "Found value: " << *it << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

    // std::list: sequence container representing a doubly-linked list
    std::list<int> lst;
    // push_back(): list container method that appends an element to the end of the list
    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);

    try {
        std::cout << "\nTesting easyfind with std::list:\n";
        // Container iterator type (std::list<int>::iterator) for bidirectional traversal
        std::list<int>::iterator it = easyfind(lst, 20);
        std::cout << "Found value: " << *it << "\n";

        it = easyfind(lst, 100);
        std::cout << "Found value: " << *it << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

    return 0;
}
