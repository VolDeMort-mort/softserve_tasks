cmake_minimum_required(VERSION 3.20)

project(Threads LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(SOURCES
    src/Reader.cpp
    src/Writer.cpp
    src/ThreadPool.cpp
    src/FileSearch.cpp
    src/Timer.cpp
)

set(HEADERS
    include/Reader.h
    include/Writer.h
    include/ThreadPool.h
    include/FileSearch.h
    include/Timer.h
    include/Scaner.h
)


add_executable(Threads
    ${SOURCES}
    ${HEADERS}
    src/main.cpp
)

target_include_directories(Threads
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)


find_package(GTest REQUIRED)
enable_testing()

file(GLOB TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/utests/*.cpp
)

if (TEST_SOURCES)
    add_executable(UTests
        ${SOURCES}
        ${HEADERS}
        ${TEST_SOURCES}
    )

    target_include_directories(UTests
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
    )

    target_link_libraries(UTests
        PRIVATE
            GTest::gtest
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(UTests)
endif()
