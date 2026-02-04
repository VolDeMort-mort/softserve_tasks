cmake_minimum_required(VERSION 3.16)
project(AnyTypeProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(anytype INTERFACE)
target_include_directories(anytype INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(anytype_app
    main.cpp
)

target_link_libraries(anytype_app
    anytype
)

add_executable(anytype_tests
    anytype_gtests.cpp
)

target_link_libraries(anytype_tests
    anytype
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(anytype_tests)

