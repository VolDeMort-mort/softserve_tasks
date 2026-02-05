cmake_minimum_required(VERSION 3.20)
project(StrList)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/str_list.cpp)
set(HEADERS include/str_list.h)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

find_package(GTest REQUIRED)
enable_testing()

file(GLOB TEST_SOURCES "utests/*.cpp")
add_executable(UTests ${TEST_SOURCES} ${SOURCES})
target_include_directories(UTests PRIVATE include)
target_link_libraries(UTests PRIVATE GTest::gtest_main GTest::gtest)

include(GoogleTest)
gtest_discover_tests(UTests)
