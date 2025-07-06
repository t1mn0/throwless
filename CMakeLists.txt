cmake_minimum_required(VERSION 3.12)
project(fpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================
# Library
# =============================================

add_library(fpp INTERFACE)

target_compile_features(fpp INTERFACE cxx_std_20)

target_include_directories(fpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(fpp INTERFACE
        -Wall -Wextra -Wpedantic
        -Wconversion -Wsign-conversion
    )
endif()

install(
    DIRECTORY include/
    DESTINATION include
)

# =============================================
# Test
# =============================================

option(BUILD_TESTING "Build tests" ON)

find_package(GTest REQUIRED)
if(NOT GTEST_FOUND)
    message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
    return()
else()
    option(BUILD_TESTS "Build tests" ON)

    if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(test/Option)
        add_subdirectory(test/Either)
    endif()
endif()