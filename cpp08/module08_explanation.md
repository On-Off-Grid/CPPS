# C++ Module 08: Templated Containers, Iterators, and Algorithms

This module is designed to introduce the **Standard Template Library (STL)** in C++98, focusing on how to efficiently use data structures, iterate over them, and apply pre-built algorithms to process data.

## Key Concepts

### 1. Templated Containers
Containers in C++ are objects that store collections of data. The STL provides various containers, and in this module, we use:
- **`std::vector`**: A dynamic array that provides fast, contiguous memory access. We use it extensively in `Span` (`ex01`) because it allows rapid sorting and element access.
- **`std::list`**: A doubly-linked list. Useful when frequent insertions or deletions are needed at both ends or in the middle. We use it to test `easyfind` (`ex00`) and for behavioral comparison in `MutantStack` (`ex02`).
- **`std::stack`**: A container adapter that provides LIFO (Last-In, First-Out) operations (`push`, `pop`, `top`). By default, it wraps a `std::deque`.

### 2. Iterators
Iterators act like pointers that allow us to traverse the elements of a container safely without knowing its underlying memory structure.
- **`iterator` / `const_iterator`**: For moving forward through a container (`begin()` to `end()`).
- **`reverse_iterator` / `const_reverse_iterator`**: For moving backward through a container (`rbegin()` to `rend()`).
- In **Exercise 02**, we unlock the power of iterators in a `std::stack`. Since `std::stack` does not expose iterators by default, we inherit from it and expose the iterators of its protected underlying container `c`.

### 3. STL Algorithms
The `<algorithm>` and `<numeric>` libraries provide powerful, optimized functions that operate on ranges of elements (using iterators). Relying on STL algorithms reduces manual loops and makes code more robust and idiomatic.
- **`std::find`**: Used in `easyfind` (`ex00`) to linearly search any container for a value.
- **`std::sort`**: Used in `Span` (`ex01`) to order numbers before finding the shortest distance between adjacent elements.
- **`std::min_element` / `std::max_element`**: Used in `Span` (`ex01`) to quickly find the absolute minimum and maximum values in the container to calculate the longest span.
- **`std::adjacent_difference`**: A powerful numeric algorithm used in `Span` (`ex01`) to map a container to the differences between its consecutive elements, significantly cleaning up the `shortestSpan` implementation.

---

## Implementations Breakdown

### Exercise 00: Easy find
**Goal:** Create a generic function to find an integer in an unknown container.
- **Implementation:** The `easyfind(T& container, int value)` template takes any container `T`. It uses `std::find(container.begin(), container.end(), value)`.
- **Error Handling:** If the iterator reaches `container.end()`, it means the value is absent, and the function throws a custom exception (`NotFoundException`). 

### Exercise 01: Span
**Goal:** Create a class that stores up to `N` integers and computes the shortest and longest spans (distances) between them.
- **Data Structure:** We use `std::vector<int>` to store the elements because sorting a contiguous array is highly optimized.
- **Bulk Insertion:** We added an `addNumbers(Iterator begin, Iterator end)` template function that uses `std::vector::insert` to quickly copy entire ranges (like appending 100,000 numbers at once).
- **Longest Span:** Computed efficiently using `std::max_element` minus `std::min_element`.
- **Shortest Span:** Computed by sorting the vector, applying `std::adjacent_difference` with a custom functor to ensure absolute difference values (represented safely as `unsigned int`), and finally querying the smallest difference using `std::min_element`.

### Exercise 02: Mutated abomination
**Goal:** Give `std::stack` iterator capabilities.
- **Context:** The standard `std::stack` is an *adapter*, meaning it doesn't manage data itself but uses an underlying container (like `std::deque`) to do the work. The standard intentionally hides this underlying container to enforce strict stack behavior (LIFO).
- **Implementation:** We create `MutantStack<T>`, which publicly inherits from `std::stack<T>`. Inside `std::stack`, the underlying container is declared as `protected` under the variable name `c`.
- **Unlocking Iterators:** By writing `this->c.begin()` and `this->c.end()`, we expose the underlying container's iterators to the outside world while maintaining all standard stack methods (`push`, `pop`, `top`). 
