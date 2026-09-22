#ifndef EASYFIND_HPP
#define EASYFIND_HPP

#include <algorithm>
#include <exception>

class NotFoundException : public std::exception {
public:
    NotFoundException() throw() {}
    NotFoundException(const NotFoundException&) throw() {}
    NotFoundException& operator=(const NotFoundException&) throw() { return *this; }
    virtual ~NotFoundException() throw() {}

    virtual const char* what() const throw() {
        return "Value not found in container";
    }
};

// Dependent type syntax: 'typename' is required when specifying a nested type (iterator) of a template parameter (T)
template <typename T>
typename T::iterator easyfind(T& container, int value) {
    // std::find algorithm (<algorithm>): performs linear search over [begin, end)
    // Returns iterator to first element equal to 'value', or container.end() if not found
    typename T::iterator it = std::find(container.begin(), container.end(), value);
    // container.end(): returns iterator to past-the-end element of the container
    if (it == container.end()) {
        throw NotFoundException();
    }
    return it;
}

// Const overload for const-qualified containers using const_iterator
template <typename T>
typename T::const_iterator easyfind(const T& container, int value) {
    // std::find algorithm (<algorithm>): operates on const iterators for read-only container access
    typename T::const_iterator it = std::find(container.begin(), container.end(), value);
    if (it == container.end()) {
        throw NotFoundException();
    }
    return it;
}

#endif
