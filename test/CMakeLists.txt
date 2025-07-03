cmake_minimum_required(VERSION 3.12)
project(fpp_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

# add_executable(test_error Error/TestError.cpp)
add_executable(test_option Option/TestOption.cpp)


target_link_libraries(
    #test_error
    test_option
    PRIVATE fpp
    PRIVATE GTest::gtest GTest::gtest_main
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test_option PRIVATE -fsanitize=address,undefined)
    target_link_options(test_option PRIVATE -fsanitize=address,undefined)
endif()

add_test(NAME test_option COMMAND test_option)