cmake_minimum_required(VERSION 3.12)
project(fpp_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

add_executable(GeneralTestEither GeneralTestEither.cpp)
add_executable(SpecializationsTest SpecializationsTest.cpp)

target_link_libraries(
    GeneralTestEither
    PRIVATE fpp
    PRIVATE GTest::gtest GTest::gtest_main
)

target_link_libraries(
    SpecializationsTest
    PRIVATE fpp
    PRIVATE GTest::gtest GTest::gtest_main
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(GeneralTestEither PRIVATE -fsanitize=address,undefined)
    target_link_options(GeneralTestEither PRIVATE -fsanitize=address,undefined)
    target_compile_options(SpecializationsTest PRIVATE -fsanitize=address,undefined)
    target_link_options(SpecializationsTest PRIVATE -fsanitize=address,undefined)
endif()

add_test(NAME GeneralTestEither COMMAND GeneralTestEither)
add_test(NAME SpecializationsTest COMMAND SpecializationsTest)