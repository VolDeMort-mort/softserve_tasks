cmake_minimum_required(VERSION 3.16)

project(task2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task2
    task2.cpp
    string_list.cpp
)

target_include_directories(task2 PRIVATE ${CMAKE_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(string_list_tests
    tests/string_list_tests.cpp
    string_list.cpp
)

target_include_directories(string_list_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(string_list_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(string_list_tests)
