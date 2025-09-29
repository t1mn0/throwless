<h1 align="center">throwless</h1>

__throwless__ is lightweight *C++20* (nearly) header-only library for functional programming without exceptions. Provides `Option`, `Result`, `SmartPtr` types for safe error handling and optional values, as well idiomatic approach in division objects and resources. __throwless__ - not only error handling and avoiding exceptions. It's more a philosophy of writing safer, more predictable code.

## Key Features
- **Exception-free error handling** - `Result<T, E>` for explicit error propagation
- **Null-Safe Optional Values** - `Option<T>` for explicit presence/absence semantics
- **Functional programming concepts**
- **Smart Memory Management** - `UniquePtr`, `SharedPtr`, `WeakPtr` with functional extensions
- **Header-only** - easy integration, no compilation required
- *C++20* compatible compiler (GCC 10+, Clang 10+)
- Tested by [gtest](https://github.com/google/googletest): [tests](test/)

## Implemented functionality
- Error concept & related primitives: [Error](Error/) - a unified Error concept that enables polymorphic error handling and seamless integration with custom error types
- Result Monad [Result](Result/) - The `Result<T, E>` type for explicit, composable error handling, deeply integrated with the Error concept for ergonomic error propagation
- Option Monad [Option](Option/) - `Option<T>` type representing optional values, implementing one of the most practical monads for null-safe programming
- Smart Pointers [SmartPtr](SmartPtr/) - `UniquePtr<T>`, `SharedPtr<T>`, `WeakPtr<T>` with supporting _RAII_ semantics

## Quick Example
```cpp
#include <iostream>
#include <string>

#include "libs/throwless/Option/Option.hpp"

tmn::err::Option<std::string> find_user_name(int user_id) {
  if (user_id == 1) return std::string("Alice");
  if (user_id == 2) return std::string("Bob");
  return tmn::err::None<std::string>();
}

int main() {
  auto name = find_user_name(3)
    .fmap([](auto s) { return "Hello, " + s + "!"; })
    .or_else([&]() {
      return std::string("User not found");
    });

  std::cout << name.value() << std::endl;
}
```

## Integration with your project
1. Download the library to your project folder:
```bash
cd your-project/libs
git clone https://github.com/t1mn0/throwless.git
```
2. Use it in the code as a pure headers:
```cpp
#include "libs/throwless/Option/Option.hpp"
```

## Building and Testing
Prerequisites:

- C++20 compiler
- CMake 3.15+
- Google Test

### Test build:
```bash
mkdir build && cd build
cmake ..
cmake --build . --parallel
```

__Running tests:__
```bash
cd build
ctest --output-on-failure
```

## Roadmap
Planned:
- More Error variations (minimum to cover all `std::exceptions`);
- A more precise hierarchy of errors that will allow them to be handled polymorphically (+ definitions and declarations of the corresponding Err-classes should be divided into files according to the hierarchies). Any user, if necessary, can use custom Errors to create their own version of the _throwless_-compatible error library;
- Add `Make-` factories-functions (`MakeUniquePtr`, `MakeShared`, etc)
- `SharedFromThis` functionality;
- To develop a global strategy for handling the presented classes with the template type `T=void` and other specific types;
- Pattern matching utilities (`Match-` syntax)
- More examples and documentation
- Chainable combinators
- Thread-safe atomic versions (..?)

## Afterword
This repository was mostly an educational one for me to practice concepts in C++, apply the knowledge I gained from functional programming, and practice GTest features that I hadn't used before.
If you see serious issue in the code, please let me know so that the malicious code does not spread.
