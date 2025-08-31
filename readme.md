<h1 align="center">throwless</h1>

__throwless__ is lightweight *C++20* library for functional programming without exceptions. Provides `Option` and `Result` types for safe error handling and optional values.

## Key Features
- **Exception-free error handling**
- **Functional programming primitives**
- **Header-only** - for easy integration
- *C++20* compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- ~~Tested by [gtest](https://github.com/google/googletest): [tests](test/)~~

## Implemented functionality
- Error concept & related primitives: [Error](Error/)
- Errors are actively used in conjunction with wrapper [Result](Result/)
- Implementation of one of the most useful _monads_: [Option](include/Option/)

## Quick Example
```cpp
☹️
```

## Installation
☹️

## Integration with your project
☹️

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
