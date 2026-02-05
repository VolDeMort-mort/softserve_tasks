#include <gtest/gtest.h>
#include <filesystem>
#include "FileAnalyzer.h"
#include "Statistics.h"
#include <fstream>     
#include <iostream> 
namespace fs = std::filesystem;

static std::string data_file(const std::string& name) {
    // шлях від кореня build до tests/data/...
    return (fs::path("tests") / "data" / name).string();
}

TEST(FileAnalyzerTests, EmptyFile) {
    FileStats s = FileAnalyzer::analyze(data_file("empty.cpp"));
    EXPECT_EQ(s.blank_lines, 0u);
    EXPECT_EQ(s.comment_lines, 0u);
    EXPECT_EQ(s.code_lines, 0u);
}

TEST(FileAnalyzerTests, OnlyBlankLines) {
    FileStats s = FileAnalyzer::analyze(data_file("only_blank.cpp"));
    EXPECT_EQ(s.blank_lines, 3u);
    EXPECT_EQ(s.comment_lines, 0u);
    EXPECT_EQ(s.code_lines, 0u);

}

TEST(FileAnalyzerTests, OnlyComments) {
    FileStats s = FileAnalyzer::analyze(data_file("only_comments.cpp"));
    EXPECT_EQ(s.blank_lines, 1u);
    EXPECT_EQ(s.comment_lines, 3u);
    EXPECT_EQ(s.code_lines, 0u);
}

TEST(FileAnalyzerTests, MultilineComment) {
    FileStats s = FileAnalyzer::analyze(data_file("multiline_comment.cpp"));
    EXPECT_EQ(s.blank_lines, 0u);
    EXPECT_EQ(s.comment_lines, 3u);
    EXPECT_EQ(s.code_lines, 0u);
}

TEST(FileAnalyzerTests, CodeWithTrailingComments) {
    FileStats s = FileAnalyzer::analyze(data_file("code_with_comments.cpp"));
    EXPECT_EQ(s.blank_lines, 0u);
    EXPECT_EQ(s.comment_lines, 0u);  
    EXPECT_EQ(s.code_lines, 4u);
}
