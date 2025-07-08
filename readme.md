__fpp/throwless__ is a small library with the implementation of modern solutions in order to encounter fewer exceptions in the cpp code.  
- Tested by [gtest](https://github.com/google/googletest)
- Implementation in the Rust / Scala / Haskell style (syntactically and semantically)
- Compliance with the functional style

The library is small, it requires improvements and improvements, as well as more test coverage. There are still parts of the code marked with a TODO comment, so if you want to add to this code, feel free to send a pull request.

In development:
- functions and objects for convenient handler call chains (Some(x), parse(), match() {Pattern Matching},  etc)
- adding asynchronous and multithreading support (atomic Option/Result/Either: thread-safe versions with std::atomic)
- stricter functional style (adding categories through concepts, analogous to traits)
- adding an analog of the 'Validation' class from the Scala language
- performance measurements, benchmarking
- add examples of using the features provided by the library, show how the library shortens the boilerplate code