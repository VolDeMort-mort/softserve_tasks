#include <gtest/gtest.h>

#include "FileAnalyzer.h"
#include "FileUtils.h"
#include "CodeAnalyzer.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

TEST(IsBlankTest, EmptyAndWhitespace) {
    EXPECT_TRUE(isBlank(""));
    EXPECT_TRUE(isBlank("   "));
    EXPECT_TRUE(isBlank("\t\t"));
    EXPECT_TRUE(isBlank(" \t "));
    EXPECT_TRUE(isBlank("\n"));
    EXPECT_TRUE(isBlank("\r\n"));
}

TEST(IsBlankTest, NonBlank) {
    EXPECT_FALSE(isBlank("a"));
    EXPECT_FALSE(isBlank(" // comment"));
    EXPECT_FALSE(isBlank("0"));
    EXPECT_FALSE(isBlank(" x "));
}

TEST(FileAnalyzerTest, CountsBasicLinesCorrectly) {
    const std::string filename = "test_basic.cpp";

    std::ofstream out(filename);
    out << "\n"
        << "// comment\n"
        << "int x = 0;\n"
        << "x++;\n";
    out.close();

    FileStatistics stats{};
    analyzeFile(filename, stats);

    EXPECT_EQ(stats.blank, 1);
    EXPECT_EQ(stats.comment, 1);
    EXPECT_EQ(stats.code, 2);

    fs::remove(filename);
}

TEST(FileAnalyzerTest, HandlesStringsWithCommentMarkers) {
    const std::string filename = "test_strings.cpp";

    std::ofstream out(filename);
    out << "std::string s = \"// not a comment\";\n"
        << "char c = '/';\n";
    out.close();

    FileStatistics stats{};
    analyzeFile(filename, stats);

    EXPECT_EQ(stats.blank, 0);
    EXPECT_EQ(stats.comment, 0);
    EXPECT_EQ(stats.code, 2);

    fs::remove(filename);
}


TEST(FileAnalyzerTest, BlockCommentEndLineCountsAsCode) {
    const std::string filename = "test_block_end.cpp";

    std::ofstream out(filename);
    out << "/* comment\n"
        << "more\n"
        << "*/\n"
        << "int x;\n";
    out.close();

    FileStatistics stats{};
    analyzeFile(filename, stats);

    EXPECT_EQ(stats.comment, 3);
    EXPECT_EQ(stats.code, 1);

    fs::remove(filename);
}

TEST(FileAnalyzerTest, MixedCodeAndCommentCountsAsCode) {
    const std::string filename = "test_mixed.cpp";

    std::ofstream out(filename);
    out << "int x = 5; // comment\n"
        << "y = 10; /* inline */\n";
    out.close();

    FileStatistics stats{};
    analyzeFile(filename, stats);

    EXPECT_EQ(stats.blank, 0);
    EXPECT_EQ(stats.comment, 0);
    EXPECT_EQ(stats.code, 2);

    fs::remove(filename);
}

TEST(FileAnalyzerTest, EscapeSequencesInStrings) {
    const std::string filename = "test_escapes.cpp";

    std::ofstream out(filename);
    out << "char* s = \"line with \\\" quote\";\n"
        << "char c = '\\'';\n";
    out.close();

    FileStatistics stats{};
    analyzeFile(filename, stats);

    EXPECT_EQ(stats.blank, 0);
    EXPECT_EQ(stats.comment, 0);
    EXPECT_EQ(stats.code, 2);

    fs::remove(filename);
}

TEST(FileAnalyzerTest, OnlyWhitespaceAfterCommentStart) {
    const std::string filename = "test_comment_whitespace.cpp";

    std::ofstream out(filename);
    out << "//\n"
        << "//   \n"
        << "/* */\n";
    out.close();

    FileStatistics stats{};
    analyzeFile(filename, stats);

    EXPECT_EQ(stats.blank, 0);
    EXPECT_EQ(stats.comment, 3);
    EXPECT_EQ(stats.code, 0);

    fs::remove(filename);
}

TEST(FileAnalyzerTest, MultilineBlockComment) {
    const std::string filename = "test_multiline_block.cpp";

    std::ofstream out(filename);
    out << "/*\n"
        << " * Comment\n"
        << " * More\n"
        << " */\n"
        << "void f() {}\n";
    out.close();

    FileStatistics stats{};
    analyzeFile(filename, stats);

    EXPECT_EQ(stats.comment, 4);
    EXPECT_EQ(stats.code, 1);

    fs::remove(filename);
}

TEST(FileAnalyzerTest, EmptyFile) {
    const std::string filename = "test_empty.cpp";

    std::ofstream out(filename);
    out.close();

    FileStatistics stats{};
    analyzeFile(filename, stats);

    EXPECT_EQ(stats.blank, 0);
    EXPECT_EQ(stats.comment, 0);
    EXPECT_EQ(stats.code, 0);

    fs::remove(filename);
}

TEST(FileAnalyzerTest, OnlyBlankLines) {
    const std::string filename = "test_only_blank.cpp";

    std::ofstream out(filename);
    out << "\n\n   \n\t\n";
    out.close();

    FileStatistics stats{};
    analyzeFile(filename, stats);

    EXPECT_EQ(stats.blank, 4);
    EXPECT_EQ(stats.comment, 0);
    EXPECT_EQ(stats.code, 0);

    fs::remove(filename);
}


TEST(FileUtilsTest, FindsOnlyCppAndHeaderFiles) {
    const std::string dir = "test_dir";
    fs::create_directory(dir);

    std::ofstream(dir + "/a.cpp").close();
    std::ofstream(dir + "/b.h").close();
    std::ofstream(dir + "/c.txt").close();
    std::ofstream(dir + "/d.hpp").close();
    std::ofstream(dir + "/e.C").close();

    auto files = findSourceFiles(dir);

    EXPECT_EQ(files.size(), 4);

    fs::remove_all(dir);
}

TEST(FileUtilsTest, HandlesEmptyDirectory) {
    const std::string dir = "empty_dir";
    fs::create_directory(dir);

    auto files = findSourceFiles(dir);
    EXPECT_TRUE(files.empty());

    fs::remove_all(dir);
}

TEST(FileUtilsTest, RecursiveSearch) {
    const std::string dir = "test_recursive";
    fs::create_directories(dir + "/subdir");

    std::ofstream(dir + "/top.cpp").close();
    std::ofstream(dir + "/subdir/nested.h").close();

    auto files = findSourceFiles(dir);

    EXPECT_EQ(files.size(), 2);

    fs::remove_all(dir);
}

TEST(FileUtilsTest, IgnoresNonRegularFiles) {
    const std::string dir = "test_special";
    fs::create_directory(dir);

    std::ofstream(dir + "/normal.cpp").close();
    fs::create_directory(dir + "/subdir");

    auto files = findSourceFiles(dir);

    EXPECT_EQ(files.size(), 1);

    fs::remove_all(dir);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    std::string path;
    std::cout << "\nEnter directory path: ";
    std::getline(std::cin, path);

    if (!path.empty()) {
        CodeAnalyzer(path).run();
    }

    return result;
}
