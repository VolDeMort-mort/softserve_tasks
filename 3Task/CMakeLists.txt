cmake_minimum_required(VERSION 3.16)
project(CppCounter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(cppcounter_lib
    Application.cpp
    FileCollector.cpp
    FileProcessor.cpp
    ThreadPool.cpp
)

target_include_directories(cppcounter_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(cppcounter_app
    main.cpp
)

target_link_libraries(cppcounter_app
    cppcounter_lib
)

add_executable(cppcounter_tests
    ApplicationTests.cpp
)

target_link_libraries(cppcounter_tests
    cppcounter_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cppcounter_tests)

