cmake_minimum_required(VERSION 3.16)
project(StringListProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(string_list
    string_list.cpp
)

target_include_directories(string_list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(string_list_app
    main.cpp
)

target_link_libraries(string_list_app
    string_list
)

add_executable(tests_string_list
    tests_string_list.cpp
)

target_link_libraries(tests_string_list
    string_list
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_string_list)

