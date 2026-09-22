#ifndef SPAN_HPP
#define SPAN_HPP

#include <vector>
#include <exception>
#include <iterator>

class Span {
private:
    unsigned int _n;
    std::vector<int> _numbers;

public:
    Span();
    Span(unsigned int n);
    Span(const Span& other);
    Span& operator=(const Span& other);
    ~Span();

    void addNumber(int number);

    // Range template method to add multiple elements using iterator range [begin, end)
    template <typename Iterator>
    void addNumbers(Iterator begin, Iterator end) {
        // std::distance (<iterator>): calculates the number of element steps between begin and end iterators
        // _numbers.size(): vector container method returning current number of stored elements
        if (std::distance(begin, end) + _numbers.size() > _n) {
            throw SpanFullException();
        }
        // vector::insert (<vector>): inserts elements from range [begin, end) before the specified position (_numbers.end())
        _numbers.insert(_numbers.end(), begin, end);
    }

    unsigned int shortestSpan() const;
    unsigned int longestSpan() const;

    class SpanFullException : public std::exception {
    public:
        virtual const char* what() const throw() {
            return "Span is already full";
        }
    };

    class SpanEmptyException : public std::exception {
    public:
        virtual const char* what() const throw() {
            return "Span does not have enough numbers to find a span";
        }
    };
};

#endif
