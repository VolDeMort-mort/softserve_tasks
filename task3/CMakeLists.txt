cmake_minimum_required(VERSION 3.14)
project(LineCounter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(main main.cpp Parser.cpp ThreadPool.cpp)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main Threads::Threads)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests.cpp Parser.cpp ThreadPool.cpp)
target_link_libraries(tests GTest::gtest_main Threads::Threads)
add_test(NAME LineCounterTests COMMAND tests)