cmake_minimum_required(VERSION 3.15)
project(fpp
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Functional Programming Primitives for C++"
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================
# Options and configuration
# =============================================
option(FPPLIB_BUILD_TESTS "Build tests" ON)
option(FPPLIB_ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" ON)
option(FPPLIB_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================
# Compiler setup
# =============================================
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic
        -Wconversion -Wsign-conversion
    )
    
    if(FPPLIB_WARNINGS_AS_ERRORS)
        add_compile_options(-Werror)
    endif()
    
    if(FPPLIB_ENABLE_SANITIZERS)
        add_compile_options(-fsanitize=address,undefined)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

# =============================================
# Library
# =============================================
add_library(fpp INTERFACE)
add_library(fpp::fpp ALIAS fpp)

target_include_directories(fpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(fpp INTERFACE cxx_std_20)

# =============================================
# Installation
# =============================================
include(GNUInstallDirs)
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# =============================================
# Test
# =============================================
if(FPPLIB_BUILD_TESTS)
    include(CTest)
    enable_testing()
    
    find_package(GTest REQUIRED)
    
    add_subdirectory(test)
endif()