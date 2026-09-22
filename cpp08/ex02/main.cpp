#include "MutantStack.hpp"
#include <iostream>
#include <list>

int main()
{
    {
        std::cout << "--- MutantStack Test ---\n";
        // MutantStack instantiation (inherits from std::stack<int>)
        MutantStack<int> mstack;
        
        // stack::push(): Pushes element onto top of stack container adaptor
        mstack.push(5);
        mstack.push(17);
        
        // stack::top(): Returns reference to top element of stack
        std::cout << "Top: " << mstack.top() << std::endl;
        
        // stack::pop(): Removes top element from stack
        mstack.pop();
        
        // stack::size(): Returns number of elements in stack container adaptor
        std::cout << "Size: " << mstack.size() << std::endl;
        
        mstack.push(3);
        mstack.push(5);
        mstack.push(737);
        //[...]
        mstack.push(0);
        
        // MutantStack iterator usage: begin() returns iterator to underlying container start
        MutantStack<int>::iterator it = mstack.begin();
        // end() returns iterator to underlying container past-the-end
        MutantStack<int>::iterator ite = mstack.end();
        
        // Iterator navigation: increment (++) and decrement (--)
        ++it;
        --it;
        while (it != ite)
        {
            // Dereference operator (*) to access container element value
            std::cout << *it << std::endl;
            ++it;
        }
        // Copy construction of base container adaptor std::stack from MutantStack
        std::stack<int> s(mstack);
    }
    
    {
        std::cout << "\n--- std::list Comparison Test ---\n";
        // std::list: doubly-linked list sequence container
        std::list<int> mlist;
        
        // list::push_back(): Appends element to end of list container
        mlist.push_back(5);
        mlist.push_back(17);
        
        // list::back(): Returns reference to last element in list container
        std::cout << "Back: " << mlist.back() << std::endl;
        
        // list::pop_back(): Removes last element from list container
        mlist.pop_back();
        
        // list::size(): Returns number of elements in list container
        std::cout << "Size: " << mlist.size() << std::endl;
        
        mlist.push_back(3);
        mlist.push_back(5);
        mlist.push_back(737);
        //[...]
        mlist.push_back(0);
        
        // list iterator usage (bidirectional iterator)
        std::list<int>::iterator it = mlist.begin();
        std::list<int>::iterator ite = mlist.end();
        
        ++it;
        --it;
        while (it != ite)
        {
            std::cout << *it << std::endl;
            ++it;
        }
    }
    
    {
        std::cout << "\n--- Additional Tests ---\n";
        MutantStack<int> mstack;
        // stack::empty(): Returns true if stack container adaptor has size 0
        std::cout << "Is empty? " << (mstack.empty() ? "Yes" : "No") << std::endl;
        
        mstack.push(42);
        mstack.push(21);
        
        MutantStack<int> mstack2(mstack);
        std::cout << "Copy constructor - mstack2 top: " << mstack2.top() << std::endl;
        
        MutantStack<int> mstack3;
        mstack3 = mstack2;
        std::cout << "Assignment operator - mstack3 top: " << mstack3.top() << std::endl;
        
        std::cout << "Iterating mstack3 backwards:\n";
        // Reverse iterator (MutantStack<int>::reverse_iterator): rbegin() to rend()
        for (MutantStack<int>::reverse_iterator rit = mstack3.rbegin(); rit != mstack3.rend(); ++rit) {
            std::cout << *rit << std::endl;
        }
    }
    return 0;
}
