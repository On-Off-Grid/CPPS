# C++98 Syntax, Libraries, and Methods Guide

This document provides a comprehensive explanation of the C++98 syntax, standard libraries, class design patterns, and methods utilized across the exercises in this codebase.

---

## 1. Class Design Patterns in C++98

C++98 lacks modern features like `deleted` functions (`= delete`) and standard helper classes. The code implements two key class design patterns using standard C++98 idioms:

### A. Static / Utility Classes (e.g., `ScalarConverter`, `Serializer`)
In C++98, a class that contains only static methods and is not intended to be instantiated must prevent instantiation and copying by making its lifecycle methods **private**:

```cpp
class ScalarConverter
{
public:
    static void convert(const std::string &literal);

private:
    ScalarConverter();                               // Private constructor
    ~ScalarConverter();                              // Private destructor
    ScalarConverter(const ScalarConverter &);        // Private copy constructor
    ScalarConverter &operator=(const ScalarConverter &); // Private assignment operator
};
```
* **Why?** Since these classes act merely as namespaces for functions, instantiating them is meaningless. In C++11, we would use `= delete`, but in C++98, we declare these constructors in the `private` section and do not implement them in the `.cpp` file. If any code tries to instantiate or copy them, the compiler flags a compilation error.

### B. Orthodox Canonical Form (e.g., `Base`, `A`, `B`, `C`)
For classes representing instantiable objects (like `Base`), C++98 enforces the **Orthodox Canonical Form**, which ensures that the class behaves correctly during copying, assignment, and destruction. It must contain:
1. Default Constructor
2. Copy Constructor
3. Copy Assignment Operator (`operator=`)
4. Destructor (virtual if the class is inherited from)

```cpp
class Base
{
public:
    virtual ~Base(); // Virtual destructor ensures derived class destructors are called
};
```

---

## 2. C++98 Standard Library Headers and Methods

The codebase relies on several standard library headers to perform type parsing, formatting, and verification.

### `<cstdlib>` — C Standard Library Utilities
Because C++98 lacks modern string-to-numeric converters like `std::stoi()`, `std::stod()`, or `std::to_string()`, it uses C-style parsing functions:

* **`std::strtol(const char *nptr, char **endptr, int base)`**:
  * Converts a string to a `long int`.
  * **`nptr`**: Pointer to the source string.
  * **`endptr`**: Address of a `char*` pointer that is updated to point to the first character *after* the parsed number.
  * **`base`**: Radix (e.g., `10` for decimal).
  * **Validation in `ScalarConverter.cpp`**:
    ```cpp
    char *end = NULL;
    long li = std::strtol(s, &end, 10);
    if (end != s && *end == '\0' && errno != ERANGE) { /* valid int */ }
    ```
    If `end` matches `s`, no digits were parsed. If `*end` is not `\0`, the string contains trailing non-numeric characters. If `errno == ERANGE`, the number overflowed/underflowed a `long`.

* **`std::strtod(const char *nptr, char **endptr)`**:
  * Converts a string to a `double`.
  * Functions exactly like `strtol`, but parses floating-point numbers (including decimal points and exponents). Used to parse `float` and `double` literals.

### `<cctype>` — Character Classification
Used to analyze and filter single characters:

* **`std::isdigit(int ch)`**: Checks if the character is a decimal digit (`'0'`-`'9'`).
* **`std::tolower(int ch)`**: Converts a character to its lowercase equivalent.
* **`std::isprint(int ch)`**: Checks if the character is printable (including spaces).
  * **Safety Tip**: Always cast the argument to `unsigned char` (e.g., `std::isprint(static_cast<unsigned char>(c))`) when dealing with `char` values, to avoid undefined behavior if the system has signed chars and the value is negative.

### `<iomanip>` and `<iostream>` — Stream I/O & Formatting
Used to output data formatted exactly as specified (e.g., displaying floats/doubles with at least one decimal digit).

* **`std::fixed`**: Sets the float formatting field of the stream to fixed-point notation (e.g., `123.4` instead of scientific `1.234e+02`).
* **`std::setprecision(int n)`**: Sets the decimal precision for floating-point values.
  * **Example usage**:
    ```cpp
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "float: " << 42.0f << "f" << std::endl; // Prints "float: 42.0f"
    ```

### `<cmath>` — Mathematics & Float Classification
In C++98, float classification functions were originally macros in `<math.h>`, which are available under `<cmath>`:

* **`std::isnan(double x)` / `std::isinf(double x)`**: Used to detect Special Floating Point Values (NaN, Positive Infinity, and Negative Infinity).
* **`std::signbit(double x)`**: Returns non-zero (true) if the sign of `x` is negative.

### `<climits>` — Type Limits
Defines limit constants for integral types:
* **`INT_MIN`**: Minimum value for an `int` (`-2147483648`).
* **`INT_MAX`**: Maximum value for an `int` (`2147483647`).
* These are used in `ScalarConverter.cpp` to verify whether a parsed `double` value can safely fit into a 32-bit `int` without overflow.

### `<stdint.h>` — Exact-width Integer Types
Defines `uintptr_t`, an unsigned integer type guaranteed to be large enough to hold a pointer value.
* **Compatibility Note**: `uintptr_t` is technically introduced in C99 / C++11, but standard compilers compiling in C++98 mode widely support `<stdint.h>` to allow safe pointer serialization.

---

## 3. C++98 Syntax Restrictions & Best Practices

When programming in C++98, you must manually handle behaviors that are automated in modern C++:

| C++98 Syntax Standard | Modern C++ Equivalent | C++98 Workaround / Solution |
| :--- | :--- | :--- |
| **No `nullptr`** | `nullptr` | Use macro `NULL` (from `<cstddef>`) or `0` for null pointers. |
| **No Type Inference** | `auto` | Always explicitly specify types (e.g., `std::string::const_iterator` instead of `auto`). |
| **No Range-based Loops** | `for (auto x : vec)` | Use traditional loops with indices or explicit iterators. |
| **No `= delete`** | `void fn() = delete;` | Declare the method in the `private:` scope and do not implement it. |
| **C++98 string parsing** | `std::stoi`, `std::stod` | Use `<cstdlib>` C functions (`std::strtol`, `std::strtod`). |
