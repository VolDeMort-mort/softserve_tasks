cmake_minimum_required(VERSION 3.16)

project(AnyTypeProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(anytype
    main.cpp
    AnyType.cpp
)

target_include_directories(anytype PRIVATE ${CMAKE_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(AnyTypeTests
    tests/AnyTypeTests.cpp
    AnyType.cpp
)

target_include_directories(AnyTypeTests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(AnyTypeTests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(AnyTypeTests)
