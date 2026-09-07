#include "MutantStack.hpp"
#include <iostream>
#include <list>

int main()
{
    {
        std::cout << "--- MutantStack Test ---\n";
        MutantStack<int> mstack;
        
        mstack.push(5);
        mstack.push(17);
        
        std::cout << "Top: " << mstack.top() << std::endl;
        
        mstack.pop();
        
        std::cout << "Size: " << mstack.size() << std::endl;
        
        mstack.push(3);
        mstack.push(5);
        mstack.push(737);
        //[...]
        mstack.push(0);
        
        MutantStack<int>::iterator it = mstack.begin();
        MutantStack<int>::iterator ite = mstack.end();
        
        ++it;
        --it;
        while (it != ite)
        {
            std::cout << *it << std::endl;
            ++it;
        }
        std::stack<int> s(mstack);
    }
    
    {
        std::cout << "\n--- std::list Comparison Test ---\n";
        std::list<int> mlist;
        
        mlist.push_back(5);
        mlist.push_back(17);
        
        std::cout << "Back: " << mlist.back() << std::endl;
        
        mlist.pop_back();
        
        std::cout << "Size: " << mlist.size() << std::endl;
        
        mlist.push_back(3);
        mlist.push_back(5);
        mlist.push_back(737);
        //[...]
        mlist.push_back(0);
        
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
        std::cout << "Is empty? " << (mstack.empty() ? "Yes" : "No") << std::endl;
        
        mstack.push(42);
        mstack.push(21);
        
        MutantStack<int> mstack2(mstack);
        std::cout << "Copy constructor - mstack2 top: " << mstack2.top() << std::endl;
        
        MutantStack<int> mstack3;
        mstack3 = mstack2;
        std::cout << "Assignment operator - mstack3 top: " << mstack3.top() << std::endl;
        
        std::cout << "Iterating mstack3 backwards:\n";
        for (MutantStack<int>::reverse_iterator rit = mstack3.rbegin(); rit != mstack3.rend(); ++rit) {
            std::cout << *rit << std::endl;
        }
    }
    return 0;
}
