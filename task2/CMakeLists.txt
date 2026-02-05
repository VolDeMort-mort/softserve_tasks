cmake_minimum_required(VERSION 3.20)
project(Structures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(GTest REQUIRED)
enable_testing()

file(GLOB TEST_SOURCES "utests/*.cpp")
add_executable(UTests ${TEST_SOURCES})
target_include_directories(UTests PRIVATE include)

target_link_libraries(UTests PRIVATE GTest::gtest_main GTest::gtest)

include(GoogleTest)
gtest_discover_tests(UTests)
