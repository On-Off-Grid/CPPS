#ifndef MUTANTSTACK_HPP
#define MUTANTSTACK_HPP

#include <stack>
#include <deque>

// Container adaptor: std::stack wraps an underlying container (default: std::deque<T>)
template <typename T, typename Container = std::deque<T> >
class MutantStack : public std::stack<T, Container> {
public:
    MutantStack() : std::stack<T, Container>() {}
    MutantStack(const MutantStack& other) : std::stack<T, Container>(other) {}
    MutantStack& operator=(const MutantStack& other) {
        if (this != &other) {
            std::stack<T, Container>::operator=(other);
        }
        return *this;
    }
    virtual ~MutantStack() {}

    // Exposing the nested iterator typedefs from the underlying container (Container)
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;
    typedef typename Container::reverse_iterator reverse_iterator;
    typedef typename Container::const_reverse_iterator const_reverse_iterator;

    // Accessing protected member 'c' of std::stack which represents the underlying container
    // returns forward iterators (begin / end)
    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
    
    // returns const forward iterators for read-only access
    const_iterator begin() const { return this->c.begin(); }
    const_iterator end() const { return this->c.end(); }

    // returns reverse iterators (rbegin / rend) for reverse traversal
    reverse_iterator rbegin() { return this->c.rbegin(); }
    reverse_iterator rend() { return this->c.rend(); }

    // returns const reverse iterators for read-only reverse traversal
    const_reverse_iterator rbegin() const { return this->c.rbegin(); }
    const_reverse_iterator rend() const { return this->c.rend(); }
};

#endif
