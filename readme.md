<h1 align="center">throwless</h1>

__throwless__ is lightweight *C++20* (nearly) header-only library for functional programming without exceptions. Provides `Option` and `Result` types for safe error handling and optional values.

## Key Features
- **Exception-free error handling**
- **Functional programming primitives**
- **Header-only** - for easy integration
- *C++20* compatible compiler (GCC 10+, Clang 10+)
- Tested by [gtest](https://github.com/google/googletest): [tests](test/)

## Implemented functionality
- Error concept & related primitives: [Error](Error/)
- Errors are actively used in conjunction with wrapper [Result](Result/)
- Implementation of one of the most useful _monads_: [Option](include/Option/)

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
In development:

- More error variations (minimum to cover all `std::exceptions`)
- Pattern matching utilities (`match` syntax)
- Chainable combinators
- Thread-safe atomic versions (?)

## Afterword
This repository was mostly an educational one for me to practice concepts in C++, apply the knowledge I gained from functional programming, and practice GTest features that I hadn't used before.
If you see serious errors in the code, please let me know so that the malicious code does not spread.
