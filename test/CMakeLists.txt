cmake_minimum_required(VERSION 3.14)

find_package(GTest REQUIRED)

set(COMMON_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/Error
    ${CMAKE_SOURCE_DIR}/Option
    ${CMAKE_SOURCE_DIR}/Result
    ${CMAKE_SOURCE_DIR}
)

set(COMMON_LINK_LIBS
    GTest::gtest
    GTest::gtest_main
)

add_library(TestUtils INTERFACE)
target_include_directories(TestUtils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(ErrorTests
    Error/ErrorTest.cpp
    Error/TryOrConvertTest.cpp
)

target_include_directories(ErrorTests PRIVATE ${COMMON_INCLUDE_DIRS})
target_link_libraries(ErrorTests PRIVATE ${COMMON_LINK_LIBS})
add_test(NAME ErrorTests COMMAND ErrorTests)

add_executable(OptionTests
    Option/GeneralTestOption.cpp
    Option/ArithmeticTestOption.cpp
)

target_include_directories(OptionTests PRIVATE ${COMMON_INCLUDE_DIRS})
target_link_libraries(OptionTests PRIVATE ${COMMON_LINK_LIBS})
add_test(NAME OptionTests COMMAND OptionTests)

add_executable(ResultTests Result/GeneralTestResult.cpp)

target_include_directories(ResultTests PRIVATE ${COMMON_INCLUDE_DIRS})
target_link_libraries(ResultTests PRIVATE ${COMMON_LINK_LIBS})
add_test(NAME ResultTests COMMAND ResultTests)
