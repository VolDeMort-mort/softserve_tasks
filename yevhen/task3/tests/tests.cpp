#include "../src/ReadLiner.h"
#include <gtest/gtest.h>

TEST(ReadLinerTest, InvalidDirectoryTest) {
    ReadLiner rl("C:/invalid/directory/path");
    ASSERT_THROW(rl.run("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen/task3/tests/test_result.csv"), std::runtime_error);
}

TEST(ReadLinerTest, BaseTest) {
    ReadLiner rl("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen");
    ASSERT_NO_THROW(rl.run("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen/task3/tests/base_test_result.csv"));
}

TEST(ReadLinerTest, BaseTestPrettyPrinter) {
    ReadLiner rl("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen");
    ASSERT_NO_THROW(rl.run("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen/task3/tests/base_test_result.txt", SaveMode::Pretty));
}

TEST(ReadLinerTest, BigProjectTest) {
    ReadLiner rl("C:/Users/kutel/.vscode/projects/cpp/godot", 255);
    ASSERT_NO_THROW(rl.run("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen/task3/tests/big_test_result.csv"));
}

TEST(ReadLinerTest, BaseTestDepth1) {
    ReadLiner rl;
    rl.setRoot("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen");
    rl.setMaxDepth(1);
    ASSERT_NO_THROW(rl.run("C:/Users/kutel/.vscode/projects/cpp/softserve_tasks/yevhen/task3/tests/base_test_result2.txt", SaveMode::Pretty));
}
