cmake_minimum_required(VERSION 3.16)
project(warmup_tasks
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SANITIZER "" CACHE STRING "Sanitizer profile: address or thread")
if(SANITIZER STREQUAL "address")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
elseif(SANITIZER STREQUAL "thread")
    add_compile_options(-fsanitize=thread,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=thread,undefined)
endif()

add_library(warmup_lib
    src/string_list.cpp
    src/line_counter.cpp
)

target_include_directories(warmup_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(Threads REQUIRED)

add_executable(line_counter src/line_counter_main.cpp)
target_link_libraries(line_counter PRIVATE warmup_lib Threads::Threads)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
