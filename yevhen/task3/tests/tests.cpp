#include "../src/ReadLiner.h"
#include <gtest/gtest.h>

TEST(ReadLinerTest, BaseTest) {
    ReadLiner rl("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen");
    ASSERT_NO_THROW(rl.run("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen/task3/tests/base_test_result.csv"));
}

TEST(ReadLinerTest, BigProjectTest) {
    ReadLiner rl("C:/Users/kutel/.vscode/projects/cpp/godot", 255);
    ASSERT_NO_THROW(rl.run("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen/task3/tests/big_test_result.csv"));
}

TEST(ReadLinerTest, InvalidDirectoryTest) {
    ReadLiner rl("C:/invalid/directory/path");
    ASSERT_THROW(rl.run("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen/task3/tests/test_result.csv"), std::runtime_error);
}