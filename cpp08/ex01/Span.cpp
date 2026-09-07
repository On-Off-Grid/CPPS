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
    if (_numbers.size() >= _n) {
        throw SpanFullException();
    }
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
    
    std::vector<int> sorted_numbers = _numbers;
    std::sort(sorted_numbers.begin(), sorted_numbers.end());
    
    std::vector<unsigned int> diffs(sorted_numbers.size());
    std::adjacent_difference(sorted_numbers.begin(), sorted_numbers.end(), diffs.begin(), SpanDiff());
    
    return *std::min_element(diffs.begin() + 1, diffs.end());
}

unsigned int Span::longestSpan() const {
    if (_numbers.size() < 2) {
        throw SpanEmptyException();
    }
    
    std::vector<int>::const_iterator min_it = std::min_element(_numbers.begin(), _numbers.end());
    std::vector<int>::const_iterator max_it = std::max_element(_numbers.begin(), _numbers.end());
    
    return static_cast<unsigned int>(*max_it) - static_cast<unsigned int>(*min_it);
}
