include(CTest)
enable_testing()

find_package(GTest REQUIRED)
if(NOT GTEST_FOUND)
    message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
    return()
endif()

function(fpp_add_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    target_link_libraries(${TEST_NAME}
        PRIVATE fpp::fpp
        PRIVATE GTest::gtest GTest::gtest_main
    )

    if(FPPLIB_ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TEST_NAME} PRIVATE -fsanitize=address,undefined)
        target_link_options(${TEST_NAME} PRIVATE -fsanitize=address,undefined)
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

file(GLOB_RECURSE TEST_SOURCES
    "Option/*.cpp"
    "Result/*.cpp"
    "Error/*.cpp"
)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    fpp_add_test(${TEST_NAME} ${TEST_SOURCE})
endforeach()
