# Modern C++ Systems & Algorithmic Engineering (C++98 Modules 06–09)

> **Portfolio Showcase**: A comprehensive suite of low-level software engineering projects demonstrating C++ memory management, type casting systems, template metaprogramming, STL container design, and algorithm benchmarking (Ford-Johnson Merge-Insert Sort).

---

## 📌 Executive Summary (For Non-Technical Recruiters & Hiring Managers)

### What is this project?
This repository showcases advanced software engineering capabilities built in **C++** under strict standards (C++98). It covers the core mechanics that power high-performance applications, game engines, financial trading platforms, and operating system components—specifically **memory safety, data structure selection, and algorithmic efficiency**.

### Key Highlights & Value Delivered:
- **Low-Level Memory Control**: Managed memory explicitly down to bit-reinterpretation and raw pointer manipulation without relying on modern garbage collectors.
- **Financial & Data Query Engines**: Built a high-speed Bitcoin exchange calculation engine that handles date-based data lookups in $O(\log N)$ logarithmic time.
- **Algorithmic Performance Benchmarking**: Implemented the comparison-optimal **Ford-Johnson Merge-Insert Sorting Algorithm** (`PmergeMe`), benchmarking how different memory structures (`std::vector` contiguous arrays vs. `std::deque` chunked memory) interact with CPU hardware caches.
- **Robust Exception Safety**: Built zero-leak, exception-safe data structures using the Orthodox Canonical Class Form.

---

## 🛠 Tech Stack & Core Competencies

- **Language Standard**: C++98 / C++03 (`-std=c++98 -Wall -Wextra -Werror`)
- **Core Concepts**:
  - **Memory & Type Management**: `static_cast`, `reinterpret_cast`, `dynamic_cast`, RTTI tables, pointer serialization (`uintptr_t`).
  - **Generic Metaprogramming**: Function/Class Templates, Compile-time polymorphism, Inclusion Model (`.tpp`).
  - **Data Structures (STL)**: Containers (`vector`, `deque`, `list`, `map`, `stack`), Iterators, Traits, Predicates.
  - **Algorithms**: Ford-Johnson Merge-Insert Sort, Binary Predecessor Search, Expression Parsers (RPN), Range Computations.
  - **Design Patterns**: Orthodox Canonical Form, Static Utility Classes, Container Adaptors, RAII.

---

## 📂 Project Modules & Architecture Breakdown

### 🔹 Module 06: C++ Type Casting & Serialization Engine
*Focus: Mastering memory interpretation, runtime type information (RTTI), and explicit conversion safety.*

| Sub-Project | Key Technologies & Concepts | Description & Achievements |
| :--- | :--- | :--- |
| **`ex00: ScalarConverter`** | Static utility classes, `static_cast`, scalar type promotion, special float literals (`nan`, `inf`) | Built a string-to-scalar literal parser that dynamically converts inputs into `char`, `int`, `float`, and `double` with overflow protection. |
| **`ex01: Serializer`** | `reinterpret_cast`, `uintptr_t`, pointer reinterpretation | Designed a memory serialization mechanism that converts complex data pointers into integer representations for raw memory transmission and back without data corruption. |
| **`ex02: Type Identification`** | `dynamic_cast`, RTTI, polymorphic base classes, pointer vs. reference failure modes | Implemented a type identification engine capable of dynamically identifying derived class types at runtime using pointers (NULL checks) and references (`std::bad_cast` exception catching) without `typeid`. |

*📖 Deep Dive Guides:* [`cpp06_complete/casting_guide.md`](./cpp06_complete/casting_guide.md) & [`cpp06_complete/cpp98_guide.md`](./cpp06_complete/cpp98_guide.md)

---

### 🔹 Module 07: Generic Programming & C++ Template Engine
*Focus: Writing reusable, type-agnostic software with zero runtime overhead.*

| Sub-Project | Key Technologies & Concepts | Description & Achievements |
| :--- | :--- | :--- |
| **`ex00: Function Templates`** | Compile-time polymorphism, template deduction, operator constraints | Created generic utility functions (`swap`, `min`, `max`) operating on any data type supporting copy construction and comparisons. |
| **`ex01: Iter Function`** | Multi-parameter templates, callback function pointers/functors, const-correctness | Designed a generic array iteration template accepting arbitrary callable entities and handling const and non-const data streams cleanly. |
| **`ex02: Array Class Template`** | Class templates, dynamic allocation (`new[]`/`delete[]`), Inclusion Model (`.tpp`), subscript `operator[]` bounds checking | Engineered a custom generic container `Array<T>` with deep copying, explicit memory lifecycle management, and out-of-bounds exception safety. |

*📖 Deep Dive Guide:* [`cpp07_complete/TEMPLATES.md`](./cpp07_complete/TEMPLATES.md)

---

### 🔹 Module 08: Standard Template Library (STL) & Iterators
*Focus: Harnessing STL sequence containers, custom iterators, and generic algorithms.*

| Sub-Project | Key Technologies & Concepts | Description & Achievements |
| :--- | :--- | :--- |
| **`ex00: Easyfind`** | STL algorithms (`std::find`), iterator traversal, dependent scope syntax (`typename T::iterator`) | Developed a container-agnostic search algorithm that works across sequential STL containers. |
| **`ex01: Span`** | Range insertion (`std::distance`, `vector::insert`), `std::adjacent_difference`, $O(N \log N)$ vs $O(N)$ spans | Implemented a high-capacity integer sequence analyzer capable of rapidly computing shortest and longest spans across tens of thousands of numbers. |
| **`ex02: MutantStack`** | Container adaptors (`std::stack`), protected member access (`this->c`), reverse iterators | Extended C++'s standard non-iterable `std::stack` to expose full iterator capabilities while retaining strict LIFO semantics. |

*📖 Deep Dive Guide:* [`cpp08/module08_explanation.md`](./cpp08/module08_explanation.md)

---

### 🔹 Module 09: Advanced Data Structures & Algorithm Design
*Focus: Real-world problem solving, time complexity trade-offs, and hardware cache performance.*

| Sub-Project | Key Technologies & Concepts | Description & Achievements |
| :--- | :--- | :--- |
| **`ex00: BitcoinExchange`** | `std::map<std::string, double>`, $O(\log N)$ predecessor search using `upper_bound`, ISO date validation | Built a financial ledger calculator. Uses an ordered map to store price histories and performs logarithmic predecessor lookups to match exact or closest past dates. |
| **`ex01: RPN Calculator`** | `std::list<double>` stack evaluation, Reverse Polish Notation parser, `stderr` error handling | Implemented a Reverse Polish Notation (RPN) mathematical expression solver using a LIFO stack architecture. |
| **`ex02: PmergeMe`** | **Ford-Johnson Merge-Insert Sort Algorithm**, Jacobsthal sequence insertion order, `std::vector` vs. `std::deque` benchmarking | Solved the comparison-optimal sorting problem. Implemented Ford-Johnson sorting across two distinct STL containers and benchmarked memory cache locality impacts on large datasets (3000+ elements). |

*📖 Deep Dive Guides:* [`cpp09/cpp09_algorithms_explained.md`](./cpp09/cpp09_algorithms_explained.md) & [`cpp09/requirements_breakdown.md`](./cpp09/requirements_breakdown.md)

---

## ⚡ Technical Case Study: Container & Hardware Performance (`PmergeMe`)

In **Module 09 (ex02)**, the goal was to implement the **Ford-Johnson Merge-Insert Sort** algorithm to minimize the total number of comparisons, and compare performance between two STL sequence containers: `std::vector` and `std::deque`.

### Performance Findings:
- **`std::vector` (Contiguous Memory)**: Outperforms `std::deque` on large datasets because all elements reside in a continuous block of physical memory. This maximizes CPU L1/L2 cache hits through sequential prefetching.
- **`std::deque` (Segmented Memory Chunks)**: Has slightly higher execution time due to pointer table indirection when traversing between non-contiguous memory chunks.

```text
Example Benchmark Output (3,000 integers):
Before: 784 12 904 33 210 ...
After:  12 33 210 784 904 ...
Time to process a range of 3000 elements with std::vector : 1420 us
Time to process a range of 3000 elements with std::deque  : 2150 us
```

---

## 🚀 Building & Running the Projects

Each exercise directory contains a dedicated `Makefile` configured with strict compilation flags:

```bash
# Compilation flags used across all modules:
clang++ -Wall -Wextra -Werror -std=c++98
```

### Quick Commands:

```bash
# Compile and run Bitcoin Exchange (Module 09 ex00)
cd cpp09/ex00
make
./btc input.csv

# Compile and run RPN Calculator (Module 09 ex01)
cd ../ex01
make
./RPN "8 9 * 9 - 9 - 4 -"

# Compile and run PmergeMe Algorithm Benchmark (Module 09 ex02)
cd ../ex02
make
./PmergeMe $(shuf -i 1-1000 -n 100)
```

---

## 🎓 Key Learnings & Engineering Growth

1. **Explicit Memory Safety**: Managed object lifecycles, memory ownership, dynamic allocation, and copy semantics without relying on modern smart pointers or automatic garbage collection.
2. **Data Structure Selection**: Mastered choosing the optimal container (`map` for predecessor lookups, `vector` for cache efficiency, `list`/`stack` for LIFO operations) based on operational complexity rather than convenience.
3. **Hardware & Cache Interaction**: Gained a practical understanding of how contiguous vs. segmented memory layouts directly impact real-world CPU performance beyond asymptotic Big-O bounds.
4. **Clean Code & Robustness**: Developed strict habits around input validation, edge-case coverage, and exception safety in production-oriented C++ code.
