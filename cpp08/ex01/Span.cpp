#include "Span.hpp"
#include <algorithm>
#include <numeric>
#include <limits>
#include <iostream>

Span::Span() : _n(0) {}

Span::Span(unsigned int n) : _n(n) {}

Span::Span(const Span& other) : _n(other._n), _numbers(other._numbers) {}

Span& Span::operator=(const Span& other) {
    if (this != &other) {
        _n = other._n;
        _numbers = other._numbers;
    }
    return *this;
}

Span::~Span() {}

void Span::addNumber(int number) {
    // vector::size(): container member function returning the current number of elements
    if (_numbers.size() >= _n) {
        throw SpanFullException();
    }
    // vector::push_back(): container member function appending an element to the end of the vector
    _numbers.push_back(number);
}

struct SpanDiff {
    unsigned int operator()(int a, int b) const {
        return static_cast<unsigned int>(a) - static_cast<unsigned int>(b);
    }
};

unsigned int Span::shortestSpan() const {
    if (_numbers.size() < 2) {
        throw SpanEmptyException();
    }
    
    // Copy the internal vector container
    std::vector<int> sorted_numbers = _numbers;
    // std::sort (<algorithm>): sorts elements in range [begin, end) into ascending order (O(N log N))
    std::sort(sorted_numbers.begin(), sorted_numbers.end());
    
    // Allocate vector container to store adjacent differences
    std::vector<unsigned int> diffs(sorted_numbers.size());
    // std::adjacent_difference (<numeric>): computes differences between consecutive elements in range [begin, end)
    // using custom binary predicate SpanDiff(), writing results starting at diffs.begin()
    std::adjacent_difference(sorted_numbers.begin(), sorted_numbers.end(), diffs.begin(), SpanDiff());
    
    // std::min_element (<algorithm>): finds iterator to the smallest element in range [diffs.begin() + 1, diffs.end())
    // Note: diffs[0] contains the first element un-differenced, so we skip it (+ 1)
    return *std::min_element(diffs.begin() + 1, diffs.end());
}

unsigned int Span::longestSpan() const {
    if (_numbers.size() < 2) {
        throw SpanEmptyException();
    }
    
    // std::min_element (<algorithm>): returns const_iterator to smallest element in container [_numbers.begin(), _numbers.end())
    std::vector<int>::const_iterator min_it = std::min_element(_numbers.begin(), _numbers.end());
    // std::max_element (<algorithm>): returns const_iterator to largest element in container [_numbers.begin(), _numbers.end())
    std::vector<int>::const_iterator max_it = std::max_element(_numbers.begin(), _numbers.end());
    
    // Dereference iterators to get values and compute span
    return static_cast<unsigned int>(*max_it) - static_cast<unsigned int>(*min_it);
}
