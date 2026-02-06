find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(warmup_tests
    string_list_test.cpp
    any_type_test.cpp
    line_counter_test.cpp
)

target_link_libraries(warmup_tests
    PRIVATE
        warmup_lib
        GTest::gtest_main
)

gtest_discover_tests(warmup_tests)
