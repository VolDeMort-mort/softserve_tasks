cmake_minimum_required(VERSION 3.16)

project(task3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task3
    src/main.cpp
    src/FileScanner.cpp
    src/Statistics.cpp
    src/FileAnalyzer.cpp
    src/ReportWriter.cpp
)

target_include_directories(task3
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

find_package(Threads REQUIRED)
target_link_libraries(task3 PRIVATE Threads::Threads)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(FileAnalyzerTests
    tests/FileAnalyzerTests.cpp
    src/FileAnalyzer.cpp
    src/Statistics.cpp
)

target_include_directories(FileAnalyzerTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(FileAnalyzerTests
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(FileAnalyzerTests)

add_custom_command(TARGET FileAnalyzerTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tests/data
        $<TARGET_FILE_DIR:FileAnalyzerTests>/tests/data
)
