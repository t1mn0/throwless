<h1 align="center">fpp/throwless</h1>

__fpp/throwless__ is a small library with the implementation of modern solutions in order to encounter fewer exceptions in the C++ code.

<h2>About</h2>

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- Tested by [gtest](https://github.com/google/googletest): [tests](test/)
- Application [examples](examples)
- Compliance with the functional style

<h2>Implemented functionality</h2>

- concept & primitives: [Error](include/Option/)
- Errors are actively used in conjunction with [Result](include/Result/)
- more general case for Result<T,E> is [Either](include/Either/)
- implementation of one of the most useful monads: [Option](include/Option/)

<h2>In development</h2>
The library is small, it requires improvements and improvements, as well as more test coverage. There are still parts of the code marked with a TODO comment, so if you want to add to this code, feel free to send a pull request.

In development:
- functions (methods) and objects for convenient handler call chains (parse(), for_each(), match() {pattern matching},  etc)
- adding asynchronous and multithreading support (atomic Option/Result/Either: thread-safe versions with std::atomic)
- stricter functional style (adding categories through concepts, analogous to traits)
- performance measurements, benchmarking