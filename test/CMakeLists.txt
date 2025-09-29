cmake_minimum_required(VERSION 3.14)

find_package(GTest REQUIRED)

set(COMMON_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/Error
    ${CMAKE_SOURCE_DIR}/Option
    ${CMAKE_SOURCE_DIR}/Result
    ${CMAKE_SOURCE_DIR}/SmartPtr/UniquePtr
    ${CMAKE_SOURCE_DIR}/SmartPtr/SharedPtr
)

set(COMMON_LINK_LIBS
    GTest::gtest
    GTest::gtest_main
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

add_executable(UniquePtrTests
    UniquePtr/GeneralTestUniquePtr.cpp
    UniquePtr/TestArrayUniquePtr.cpp
)

target_include_directories(UniquePtrTests PRIVATE ${COMMON_INCLUDE_DIRS})
target_link_libraries(UniquePtrTests PRIVATE ${COMMON_LINK_LIBS})
add_test(NAME UniquePtrTests COMMAND UniquePtrTests)

add_executable(SharedPtrTests
    SharedPtr/GeneralTestSharedPtr.cpp
    SharedPtr/TestArraySharedPtr.cpp
)

target_include_directories(SharedPtrTests PRIVATE ${COMMON_INCLUDE_DIRS})
target_link_libraries(SharedPtrTests PRIVATE ${COMMON_LINK_LIBS})
add_test(NAME SharedPtrTests COMMAND SharedPtrTests)

add_executable(WeakPtrTests
    WeakPtr/GeneralTestWeakPtr.cpp
    WeakPtr/TestArrayWeakPtr.cpp
)

target_include_directories(WeakPtrTests PRIVATE ${COMMON_INCLUDE_DIRS})
target_link_libraries(WeakPtrTests PRIVATE ${COMMON_LINK_LIBS})
add_test(NAME WeakPtrTests COMMAND WeakPtrTests)
