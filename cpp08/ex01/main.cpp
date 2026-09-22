#include "Span.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    std::cout << "--- Subject tests ---\n";
    Span sp = Span(5);
    sp.addNumber(6);
    sp.addNumber(3);
    sp.addNumber(17);
    sp.addNumber(9);
    sp.addNumber(11);
    std::cout << "Shortest span: " << sp.shortestSpan() << std::endl;
    std::cout << "Longest span: " << sp.longestSpan() << std::endl;

    std::cout << "\n--- Exception tests ---\n";
    Span sp_empty(5);
    try {
        sp_empty.shortestSpan();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    try {
        sp_empty.addNumber(1);
        sp_empty.longestSpan();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    try {
        sp_empty.addNumber(2);
        sp_empty.addNumber(3);
        sp_empty.addNumber(4);
        sp_empty.addNumber(5);
        sp_empty.addNumber(6); // Should throw
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    std::cout << "\n--- Large range test (100,000 elements) ---\n";
    Span sp_large(100000);
    // std::vector: sequence container holding dynamically populated data
    std::vector<int> large_vec;
    std::srand(std::time(NULL));
    for (int i = 0; i < 100000; ++i) {
        // vector::push_back(): container method appending element to vector
        large_vec.push_back(std::rand());
    }
    
    try {
        // Passing iterator range [large_vec.begin(), large_vec.end()) to template addNumbers
        sp_large.addNumbers(large_vec.begin(), large_vec.end());
        std::cout << "Successfully added 100,000 numbers.\n";
        std::cout << "Shortest span: " << sp_large.shortestSpan() << "\n";
        std::cout << "Longest span: " << sp_large.longestSpan() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
    
    std::cout << "\n--- Add range exception test ---\n";
    Span sp_small(10);
    try {
        // Random access iterator arithmetic (large_vec.begin() + 15) to pass range of 15 elements
        sp_small.addNumbers(large_vec.begin(), large_vec.begin() + 15);
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    std::cout << "\n--- Negative and duplicate values test ---\n";
    Span sp_neg(5);
    sp_neg.addNumber(-10);
    sp_neg.addNumber(-20);
    sp_neg.addNumber(5);
    sp_neg.addNumber(5);
    sp_neg.addNumber(100);
    std::cout << "Shortest span (with duplicates): " << sp_neg.shortestSpan() << "\n";
    std::cout << "Longest span (with negatives): " << sp_neg.longestSpan() << "\n";

    return 0;
}
