cmake_minimum_required(VERSION 3.14)
project(StringList)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

add_library(stringlist StringList.c)
target_include_directories(stringlist PUBLIC ${CMAKE_SOURCE_DIR})
add_executable(main main.c)
target_link_libraries(main stringlist)


include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests/stringListTests.cpp)
target_link_libraries(tests stringlist GTest::gtest_main)
add_test(NAME StringListTests COMMAND tests)
