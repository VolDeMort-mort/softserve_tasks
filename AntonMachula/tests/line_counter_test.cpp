#include <gtest/gtest.h>

#include "line_counter.h"
#include <filesystem>
#include <fstream>

class LineCounterTest : public ::testing::Test {
protected:
    std::filesystem::path tmp_dir;

    void SetUp() override {
        tmp_dir = std::filesystem::temp_directory_path() / "line_counter_test";
        std::filesystem::create_directories(tmp_dir);
    }

    void TearDown() override {
        std::filesystem::remove_all(tmp_dir);
    }

    std::string write_file(const std::string& name, const std::string& content) {
        auto path = tmp_dir / name;
        std::ofstream out(path);
        out << content;
        return path.string();
    }
};

TEST_F(LineCounterTest, EmptyFile) {
    auto path = write_file("empty.cpp", "");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.blank_lines, 0);
    EXPECT_EQ(stats.comment_lines, 0);
    EXPECT_EQ(stats.code_lines, 0);
}

TEST_F(LineCounterTest, BlankLinesOnly) {
    auto path = write_file("blank.cpp", "\n   \n\t\n");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.blank_lines, 3);
    EXPECT_EQ(stats.comment_lines, 0);
    EXPECT_EQ(stats.code_lines, 0);
}

TEST_F(LineCounterTest, CodeOnly) {
    auto path = write_file("code.cpp", "int x = 5;\nreturn x;\n");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.code_lines, 2);
    EXPECT_EQ(stats.comment_lines, 0);
    EXPECT_EQ(stats.blank_lines, 0);
}

TEST_F(LineCounterTest, SingleLineComments) {
    auto path = write_file("comments.cpp", "// comment one\n  // indented comment\n");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.comment_lines, 2);
    EXPECT_EQ(stats.code_lines, 0);
}

TEST_F(LineCounterTest, BlockComment) {
    auto path = write_file("block.cpp", "/* line one\n   line two\n   line three */\n");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.comment_lines, 3);
    EXPECT_EQ(stats.code_lines, 0);
}

TEST_F(LineCounterTest, CodeWithTrailingComment) {
    auto path = write_file("mixed.cpp", "int x = 5; // init\n");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.code_lines, 1);
    EXPECT_EQ(stats.comment_lines, 1);
}

TEST_F(LineCounterTest, SingleLineBlockComment) {
    auto path = write_file("single_block.cpp", "/* just a comment */\n");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.comment_lines, 1);
    EXPECT_EQ(stats.code_lines, 0);
}

TEST_F(LineCounterTest, BlockCommentCloseThenCode) {
    auto path = write_file("close_then_code.cpp", "/* comment\n*/ int x;\n");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.comment_lines, 2);
    EXPECT_EQ(stats.code_lines, 1);
}

TEST_F(LineCounterTest, StringLiteralWithSlashes) {
    auto path = write_file("string.cpp", "auto url = \"http://example.com\";\n");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.code_lines, 1);
    EXPECT_EQ(stats.comment_lines, 0);
}

TEST_F(LineCounterTest, CharLiteralSlash) {
    auto path = write_file("char.cpp", "char c = '/';\n");
    auto stats = count_lines_in_file(path);
    EXPECT_EQ(stats.code_lines, 1);
    EXPECT_EQ(stats.comment_lines, 0);
}

TEST_F(LineCounterTest, NonExistentFileThrows) {
    EXPECT_THROW(count_lines_in_file("/nonexistent/path.cpp"), std::runtime_error);
}
