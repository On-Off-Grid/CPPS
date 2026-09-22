# C++ Templates & Project Use Cases Guide

This guide explains the concepts of C++ templates in general and their specific implementations and use cases in this project.

---

## 1. Templates in General

### What is a Template?
A **template** in C++ is a blueprint or formula for creating classes or functions. It is the foundation of **Generic Programming**, which allows writing code that is independent of any particular type. 

Instead of writing separate overloaded functions or separate classes for every data type (e.g., `int`, `double`, `std::string`), you write a single template. The compiler then generates the concrete code for the required types at compile-time.

```cpp
template <typename T>
T add(T a, T b) {
    return a + b;
}
```

### Key Concepts

1. **Template Parameters**: The placeholders for types or values, defined inside `<...>`. 
   - `typename` or `class` is used to specify a type parameter (e.g., `typename T`).
   - Non-type template parameters can also be passed (e.g., `template <typename T, int Size>`).
2. **Template Instantiation**: The process by which the compiler generates a concrete class or function from a template.
   - **Implicit Instantiation**: The compiler automatically determines the type arguments (e.g., `add(5, 3)` instantiates `add<int>`).
   - **Explicit Instantiation**: The programmer explicitly specifies the types (e.g., `add<double>(5.5, 3.2)`).
3. **Compile-Time Polymorphism**: Unlike runtime inheritance/virtual functions, templates resolve polymorphism at compile-time. There is zero runtime performance overhead, but it can lead to **code bloat** (multiple copies of the code generated for different types) and longer compile times.
4. **Duck Typing at Compile-Time**: The template code assumes that the parameterized type `T` supports whatever operations are performed on it (e.g., `+`, `<`, assignment). If a type `T` does not support these operations, a compile-time error occurs.

---

## 2. Compilation and the Inclusion Model (`.tpp` Files)

Normally, C++ source code is split into declaration headers (`.hpp`) and implementation source files (`.cpp`). However, templates do not work this way because **templates are not code; they are instructions for the compiler to generate code**.

When the compiler compiles a file that uses a template, it needs to see both the template declaration and the implementation to instantiate the code. If the template implementation is in a separate `.cpp` file that is compiled independently, the compiler will not generate the template code for the other file, resulting in a linker error (`undefined reference`).

### The Solution: The Inclusion Model (`.tpp` files)
To keep declarations clean while making implementation details available:
1. Define the class interface in the `.hpp` file.
2. Put the template implementation in a separate template parameter file, traditionally named `.tpp`.
3. Include the `.tpp` file at the bottom of the `.hpp` file.

This is demonstrated in this project under [ex02](./ex02):
- [`Array.hpp`](./ex02/Array.hpp) defines the [`Array`](./ex02/Array.hpp#L7-L30) class interface.
- [`Array.tpp`](./ex02/Array.tpp) implements its constructors, destructor, and operators.
- At the bottom of `Array.hpp`, `#include "Array.tpp"` ensures the implementation is visible during compilation.

---

## 3. Project Use Cases and Implementations

### Exercise 00: Function Templates (`swap`, `min`, `max`)
Located in [ex00](./ex00).

This exercise introduces the basic syntax of function templates. It implements three functions:
- [`swap`](./ex00/whatever.hpp#L4-L10): Swaps the values of two arguments.
- [`min`](./ex00/whatever.hpp#L12-L16): Returns the smaller of two values. If they are equal, it returns the second one.
- [`max`](./ex00/whatever.hpp#L18-L22): Returns the greater of two values. If they are equal, it returns the second one.

#### Constraints on Type `T`:
For these functions to compile and run successfully, any type passed to them must support:
- Copy construction and copy assignment (needed for `swap`'s temporary variable).
- Comparison operators: `operator<` (for `min`) and `operator>` (for `max`).

---

### Exercise 01: Multi-parameter Function Template (`iter`)
Located in [ex01](./ex01).

This exercise defines a function template [`iter`](./ex01/iter.hpp#L6-L18) that applies a function to each element of an array:
```cpp
template <typename T, typename F>
void iter(T* array, const std::size_t length, F function);
```

#### Why use two template parameters (`T` and `F`)?
- `T` represents the type of the elements in the array (e.g. `int`, `std::string`).
- `F` represents the type of the callable function/functor. Rather than constraining the third parameter to a rigid function pointer signature (e.g. `void (*f)(T&)`), using a template parameter `F` allows `iter` to accept function pointers, functors, or lambdas with compatible signatures.

#### Const and Non-Const Overloads:
To ensure type safety and const-correctness, `iter` is overloaded:
1. One version accepting `T* array` (allowing modifications to array elements).
2. One version accepting `const T* array` (read-only iteration, suitable for const arrays).

---

### Exercise 02: Class Template (`Array`)
Located in [ex02](./ex02).

This exercise implements a generic container class template [`Array`](./ex02/Array.hpp#L7-L30) that wraps a dynamically-allocated array of type `T`.

#### Key Features:
1. **Dynamic Allocation**: Elements are allocated via `new T[n]()`, which default-initializes them.
2. **Orthodox Canonical Form**:
   - **Default Constructor**: Initializes an empty array (`_size = 0`, `_data = NULL`).
   - **Parameterized Constructor**: Initializes an array of size `n`.
   - **Copy Constructor / Assignment Operator**: Performs a **deep copy**. Changing elements of one array does not affect the other.
   - **Destructor**: Properly releases dynamically allocated memory using `delete[] _data`.
3. **Subscript Operator (`operator[]`)**:
   - Overloaded for both non-const and const objects.
   - Performs **bounds checking**. If the index is out of bounds, it throws a custom `OutOfBoundsException`.
4. **Size Getter**: A const member function `size()` returning the number of elements.
