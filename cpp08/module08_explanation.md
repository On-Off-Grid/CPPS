# C++ Module 08: Templated Containers, Iterators, and Algorithms

This module is designed to introduce the **Standard Template Library (STL)** in C++98, focusing on how to efficiently use data structures, iterate over them, and apply pre-built algorithms to process data.

---

## Key Concepts

### 1. Templated Containers
Containers in C++ are generic data structures that manage elements of a given type. In this module:
- **`std::vector<T>`**: A sequence container encapsulating a dynamic contiguous array. Provides $O(1)$ random access, fast iteration, and continuous memory allocation. Used in `Span` (`ex01`) for fast element access and sorting.
- **`std::list<T>`**: A sequence container representing a doubly-linked list. Provides $O(1)$ insertions and deletions anywhere in the sequence, with non-contiguous memory allocations. Used to test `easyfind` (`ex00`) and for behavioral comparison in `MutantStack` (`ex02`).
- **`std::stack<T, Container>`**: A container adaptor offering a LIFO (Last-In, First-Out) interface (`push`, `pop`, `top`). By default, it wraps `std::deque<T>`.

### 2. Iterators
Iterators act as an abstraction over memory pointers, providing a unified interface to traverse elements across different container types:
- **Forward Iterators (`iterator` / `const_iterator`)**: Traverse elements from start to end (`begin()` to `end()`).
- **Reverse Iterators (`reverse_iterator` / `const_reverse_iterator`)**: Traverse elements in reverse order (`rbegin()` to `rend()`).
- **Dependent Type Syntax (`typename Container::iterator`)**: When referencing a nested type dependent on a template parameter, the `typename` keyword must precede the qualifier.

### 3. STL Algorithms
The `<algorithm>` and `<numeric>` headers provide template functions that operate on ranges defined by iterators `[first, last)`:
- **`std::find`**: Performs linear search for a value.
- **`std::sort`**: Sorts elements in ascending order ($O(N \log N)$ complexity).
- **`std::min_element` / `std::max_element`**: Finds iterators to the minimum or maximum element in a range.
- **`std::adjacent_difference`**: Computes differences between consecutive elements in a sequence.

---

## Detailed Exercise Breakdown & C++ Syntax Guide

---

### Exercise 00: Easy find

**Goal:** Create a generic templated function `easyfind` to locate the first occurrence of an integer in an unknown STL container.

#### C++ Syntax & Method Breakdown

1. **Dependent Scope Type Syntax:**
   ```cpp
   template <typename T>
   typename T::iterator easyfind(T& container, int value);
   
   template <typename T>
   typename T::const_iterator easyfind(const T& container, int value);
   ```
   - `template <typename T>`: Declares `easyfind` as a function template parameterized by container type `T`.
   - `typename T::iterator`: The `typename` keyword informs the compiler that `iterator` is a type nested inside template parameter `T`.
   - Const overload permits function calls on `const T&` using `const_iterator`.

2. **`<algorithm>` Search Function (`std::find`):**
   ```cpp
   typename T::iterator it = std::find(container.begin(), container.end(), value);
   ```
   - **Signature:** `template<class InputIterator, class T> InputIterator find(InputIterator first, InputIterator last, const T& val);`
   - **Iterators:** `container.begin()` returns an iterator to the first element; `container.end()` returns past-the-end sentinel iterator.
   - **Return Value:** Returns iterator to first element equal to `value`, or `container.end()` if not found.

3. **Error Handling & Exception Syntax:**
   ```cpp
   if (it == container.end()) {
       throw NotFoundException();
   }
   ```
   - Inherits from `std::exception` and overrides `virtual const char* what() const throw()`.

---

### Exercise 01: Span

**Goal:** Create a class `Span` that stores up to `N` integers and computes the shortest and longest spans (differences) between stored values.

#### C++ Syntax & Method Breakdown

1. **Range Insertion with Template Member Functions:**
   ```cpp
   template <typename Iterator>
   void addNumbers(Iterator begin, Iterator end) {
       if (std::distance(begin, end) + _numbers.size() > _n) {
           throw SpanFullException();
       }
       _numbers.insert(_numbers.end(), begin, end);
   }
   ```
   - **`std::distance(begin, end)` (`<iterator>`):** Computes distance (number of elements) between two iterators. Works with forward, bidirectional, and random access iterators.
   - **`vector::insert` (`<vector>`):** `_numbers.insert(pos, first, last)` inserts range `[first, last)` before iterator position `pos` (`_numbers.end()`).

2. **Custom Binary Predicate / Functor Syntax:**
   ```cpp
   struct SpanDiff {
       unsigned int operator()(int a, int b) const {
           return static_cast<unsigned int>(a) - static_cast<unsigned int>(b);
       }
   };
   ```
   - Overloads function call `operator()` to transform difference computation into unsigned types without overflow.

3. **Shortest Span Algorithm Syntax (`std::sort` & `std::adjacent_difference`):**
   ```cpp
   std::vector<int> sorted_numbers = _numbers;
   std::sort(sorted_numbers.begin(), sorted_numbers.end());
   
   std::vector<unsigned int> diffs(sorted_numbers.size());
   std::adjacent_difference(sorted_numbers.begin(), sorted_numbers.end(), diffs.begin(), SpanDiff());
   
   return *std::min_element(diffs.begin() + 1, diffs.end());
   ```
   - **`std::sort(first, last)` (`<algorithm>`):** Sorts range `[begin, end)` into ascending order using operator `<`.
   - **`std::adjacent_difference(first, last, result, op)` (`<numeric>`):** Calculates differences between adjacent elements $diff[i] = op(val[i], val[i-1])$ and stores them into output iterator `diffs.begin()`.
   - **`std::min_element(first, last)` (`<algorithm>`):** Finds iterator to minimum element in range `[diffs.begin() + 1, diffs.end())`. The `+ 1` offset skips `diffs[0]` which contains `sorted_numbers[0]`.

4. **Longest Span Algorithm Syntax (`std::min_element` & `std::max_element`):**
   ```cpp
   std::vector<int>::const_iterator min_it = std::min_element(_numbers.begin(), _numbers.end());
   std::vector<int>::const_iterator max_it = std::max_element(_numbers.begin(), _numbers.end());
   return static_cast<unsigned int>(*max_it) - static_cast<unsigned int>(*min_it);
   ```
   - **`std::max_element(first, last)` (`<algorithm>`):** Returns iterator pointing to element with maximum value.
   - Computes distance between extreme elements in $O(N)$ time without full sort.

---

### Exercise 02: Mutated Abomination (`MutantStack`)

**Goal:** Extend `std::stack` to expose iterator support while retaining standard LIFO stack semantics.

#### C++ Syntax & Method Breakdown

1. **Inheritance from Container Adaptors:**
   ```cpp
   template <typename T, typename Container = std::deque<T> >
   class MutantStack : public std::stack<T, Container> { ... };
   ```
   - `std::stack` is a container adaptor defined as `template <class T, class Container = deque<T> > class stack;`.
   - Derived class `MutantStack` publicly inherits from `std::stack<T, Container>`.

2. **Accessing Protected Member `c`:**
   ```cpp
   iterator begin() { return this->c.begin(); }
   iterator end() { return this->c.end(); }
   ```
   - Standard `std::stack` contains a `protected` member variable `c` of type `Container`.
   - Using `this->c`, `MutantStack` gains direct access to the underlying container object and delegates `begin()`, `end()`, `rbegin()`, and `rend()` calls directly to `c`.

3. **Exposing Container Typedefs:**
   ```cpp
   typedef typename Container::iterator iterator;
   typedef typename Container::const_iterator const_iterator;
   typedef typename Container::reverse_iterator reverse_iterator;
   typedef typename Container::const_reverse_iterator const_reverse_iterator;
   ```
   - Alias nested container iterator types into `MutantStack` interface.

4. **Comparing Container Adaptors vs Sequence Containers:**
   | Syntax / Method | `std::stack<T>` / `MutantStack<T>` | `std::list<T>` |
   | :--- | :--- | :--- |
   | Add Element | `mstack.push(val)` | `mlist.push_back(val)` |
   | Remove Element | `mstack.pop()` | `mlist.pop_back()` |
   | Inspect End Element | `mstack.top()` | `mlist.back()` |
   | Iterator Traversal | Exposed via `MutantStack` `begin()` / `end()` | Native `begin()` / `end()` |
