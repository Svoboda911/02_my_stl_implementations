# MyVector - Custom C++ Dynamic Array

A personal C++ project implementing a simplified version of `std::vector` from scratch.

The main goal of this project is to understand how dynamic containers work internally and gain practical experience with memory management, object lifetime, allocators, and modern C++ features.

## Features

- **Dynamic memory allocation** using `std::allocator_traits`.
- **Manual object lifetime management** using `construct()` and `destroy()`.
- **Automatic capacity management** when adding new elements.
- **Exception handling** during memory allocation and object construction.
- **Move semantics** for transferring existing elements during reallocation.
- **Perfect forwarding** through variadic templates and `std::forward`.
- **Initializer list support** for convenient initialization.

## Implemented Methods

| Method | Description |
|---|---|
| `MyVector()` | Creates an empty vector |
| `MyVector(size_t)` | Creates a vector with a specified number of default-constructed elements |
| `MyVector(initializer_list<T>)` | Initializes a vector from an initializer list |
| `size()` | Returns the number of stored elements |
| `operator[]` | Provides direct element access |
| `at()` | Provides bounds-checked element access |
| `reserve()` | Allocates additional capacity |
| `resize()` | Changes the number of elements |
| `push_back()` | Adds an element to the end |
| `emplace_back()` | Constructs an element directly in allocated memory |
| `pop_back()` | Removes the last element |
| `shrink_to_fit()` | Reduces capacity to match the current size |
| `~MyVector()` | Destroys elements and releases allocated memory |

## Technical Concepts

This project explores several important C++ concepts:

- Templates and variadic templates
- `std::allocator` and `std::allocator_traits`
- Placement construction and object lifetime
- Dynamic memory allocation and deallocation
- RAII (Resource Acquisition Is Initialization)
- Move semantics and `std::move`
- Perfect forwarding and `std::forward`
- Exception safety and resource cleanup

## Rule of Three

Implemented the Rule of Three to ensure proper resource management and object lifetime control.
The container provides:
| `Destructor (~MyVector)` | — destroys all constructed elements and releases allocated memory. |
| `Copy Constructor` | — performs deep copying by allocating independent memory and copy-constructing each element. |
| `Copy Assignment Operator (operator=)` | — replaces the contents of an existing container with a deep copy of another container. |

## Key Concepts
- Deep copying and independent memory ownership.
- Manual object lifetime management using std::allocator_traits.
- Raw memory allocation and deallocation.
- Exception safety during element construction.
- Self-assignment protection.
- Proper cleanup of partially constructed objects when exceptions occur.

## Exception Safety
The copy operations are designed to prevent memory leaks and preserve the original container state if element copying fails.
Goal: Understand and implement the resource management principles behind standard C++ containers rather than relying on ready-made container implementations.

## Example Usage

```cpp
#include <iostream>
#include "MyVector.h"

int main() {
    MyVector<int> numbers{10, 20, 30};

    numbers.push_back(40);
    numbers.emplace_back(50);

    numbers.resize(3);

    for (std::size_t i = 0; i < numbers.size(); ++i) {
        std::cout << numbers[i] << '\n';
    }

    return 0;
}
```

Output:

```text
10
20
30
```

## Project Status

This project is under development.

The current implementation focuses on dynamic memory management, element construction and destruction, and basic vector operations.

Further improvements will include additional container functionality, iterator support, copy and move operations, and more comprehensive exception-safety guarantees.

## Motivation

I developed this project as part of my independent C++ studies to gain a deeper understanding of how standard library containers work internally rather than relying exclusively on their interfaces.

The project is intended for educational purposes and is not a replacement for `std::vector` in production applications.



cmake -S . -B build -G Ninja
cmake --build build
