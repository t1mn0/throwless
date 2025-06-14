cmake_minimum_required(VERSION 3.12)
project(throwless LANGUAGES CXX)

# the interface library (header-only)
add_library(throwless INTERFACE)

target_include_directories(throwless INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(
    DIRECTORY include/
    DESTINATION include
)