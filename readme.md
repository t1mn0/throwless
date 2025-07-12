<h1 align="center">fpp/throwless</h1>

__fpp/throwless__ is a small library with the implementation of modern solutions in order to encounter fewer exceptions in the C++ code.

## Key Features
- **Exception-free error handling**
- **Functional programming primitives**
- **Header-only** - for easy integration
- *C++20* compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- Tested by [gtest](https://github.com/google/googletest): [tests](test/)
- Application [examples](examples)

## Implemented functionality

- Error concept & related primitives: [Error](include/Option/)
- Errors are actively used in conjunction with wrapper [Result](include/Result/)
- More general case for Result<T,E> is [Either](include/Either/)
- Implementation of one of the most useful monads: [Option](include/Option/)

## Quick Example

```cpp
#include <fpp/Result.hpp>
#include <iostream>

using namespace fpp;

Result<int, StringError> safe_divide(int a, int b) {
    if (b == 0) return Result<int, StringError>::Err(StringError("Division by zero"));
    return Result<int, StringError>::Ok(a / b);
}

int main() {
    const auto result = safe_divide(10, 2)
        .map([](int x) { return x * 2; })
        .and_then([](int x) {
            return std::to_string(x + 1);
        });

    if (result.is_ok()){
        std::cout << "Success: " << result.unwrap_or(0); 
    } else {
        std::cerr << "Error: " << result.unwrap_err_or_exception().err_message();
    }
}
```

## Building and Testing
Prerequisites:

- C++20 compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.15+
- Google Test (for tests)

### Basic build
```bash
mkdir build && cd build
cmake ..
cmake --build . --parallel
```

__Build options:__

- DFPPLIB_BUILD_TESTS=ON/OFF - Build tests (*default: ON*)
- DFPPLIB_ENABLE_SANITIZERS=ON/OFF - Enable sanitizers (*default: ON*)
- DFPPLIB_WARNINGS_AS_ERRORS=ON/OFF - Treat warnings as errors (*default: ON*)

__Running tests:__
```bash
cd build
ctest --output-on-failure
```

## Roadmap
In development:

- Pattern matching utilities (`match` syntax)
- Chainable combinators
- Better monadic interface
- Thread-safe atomic versions
- Perfomance/benchmarking suite