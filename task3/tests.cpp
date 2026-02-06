#include "Config.h"
#include "Parser.h"
#include "ThreadPool.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <atomic>

namespace fs = std::filesystem;

class ParserTest : public ::testing::Test {
protected:
    std::string testDir;

    void SetUp() override {
        testDir = fs::temp_directory_path().string() + "/lineCounterTests";
        fs::create_directories(testDir);
    }

    void TearDown() override {
        std::error_code ec;
        fs::remove_all(testDir, ec);
    }

    void writeFile(const std::string& name, const std::string& content) {
        std::ofstream file(testDir + "/" + name);
        file << content;
    }
};

TEST_F(ParserTest, GivenBlankLine_WhenChecked_ThenReturnsTrue) {
    EXPECT_TRUE(isBlankLine(""));
    EXPECT_TRUE(isBlankLine("   "));
    EXPECT_TRUE(isBlankLine("\t\t"));
    EXPECT_TRUE(isBlankLine("  \t  "));
    EXPECT_FALSE(isBlankLine("int x;"));
}

TEST_F(ParserTest, GivenSingleLineComments_WhenParsed_ThenCountedAsComments) {
    writeFile("comments.cpp", "// comment 1\n// comment 2\n// comment 3\n");
    auto stats = parseFile(testDir + "/comments.cpp");
    EXPECT_EQ(stats.commentLines, 3u);
    EXPECT_EQ(stats.codeLines, 0u);
    EXPECT_EQ(stats.blankLines, 0u);
}

TEST_F(ParserTest, GivenMultiLineComments_WhenParsed_ThenCountedAsComments) {
    writeFile("multicomment.cpp", "/* start\n   middle\n   end */\n");
    auto stats = parseFile(testDir + "/multicomment.cpp");
    EXPECT_EQ(stats.commentLines, 3u);
    EXPECT_EQ(stats.codeLines, 0u);
}

TEST_F(ParserTest, GivenCodeLines_WhenParsed_ThenCountedAsCode) {
    writeFile("code.cpp", "#include <iostream>\nint main() {\n    return 0;\n}\n");
    auto stats = parseFile(testDir + "/code.cpp");
    EXPECT_EQ(stats.codeLines, 4u);
    EXPECT_EQ(stats.commentLines, 0u);
}

TEST_F(ParserTest, GivenMixedCodeComment_WhenParsed_ThenCountedAsCode) {
    writeFile("mixed.cpp", "int x = 0; // initialize\nint y = 1; /* value */\n");
    auto stats = parseFile(testDir + "/mixed.cpp");
    EXPECT_EQ(stats.codeLines, 2u);
    EXPECT_EQ(stats.commentLines, 0u);
}

TEST_F(ParserTest, GivenEmptyFile_WhenParsed_ThenAllCountsZero) {
    writeFile("empty.cpp", "");
    auto stats = parseFile(testDir + "/empty.cpp");
    EXPECT_EQ(stats.blankLines, 0u);
    EXPECT_EQ(stats.commentLines, 0u);
    EXPECT_EQ(stats.codeLines, 0u);
}

TEST_F(ParserTest, GivenBinaryFile_WhenParsed_ThenReturnsEmptyStats) {
    std::string binPath = testDir + "/binary.cpp";
    std::ofstream f(binPath, std::ios::binary);
    char data[] = {0x7f, 0x45, 0x4c, 0x46, 0x00, 0x01};
    f.write(data, sizeof(data));
    f.close();
    auto stats = parseFile(binPath);
    EXPECT_EQ(stats.totalLines(), 0u);
}

TEST_F(ParserTest, GivenBlankLinesMixedIn_WhenParsed_ThenBlankLinesCounted) {
    writeFile("blanks.cpp", "int x;\n\nint y;\n\n\n");
    auto stats = parseFile(testDir + "/blanks.cpp");
    EXPECT_EQ(stats.codeLines, 2u);
    EXPECT_EQ(stats.blankLines, 3u);
}

class ConfigTest : public ParserTest {};

TEST_F(ConfigTest, GivenValidConfig_WhenLoaded_ThenValuesCorrect) {
    writeFile("test_config.txt", "extensions=.c,.h\nthreadCount=8\noutputFilePath=result.csv\n");
    Config cfg;
    bool ok = cfg.loadFromFile(testDir + "/test_config.txt");
    EXPECT_TRUE(ok);
    EXPECT_EQ(cfg.extensions.size(), 2u);
    EXPECT_EQ(cfg.extensions[0], ".c");
    EXPECT_EQ(cfg.extensions[1], ".h");
    EXPECT_EQ(cfg.threadCount, 8u);
    EXPECT_EQ(cfg.outputFilePath, "result.csv");
}

TEST_F(ConfigTest, GivenMissingConfig_WhenLoaded_ThenReturnsFalseWithDefaults) {
    Config cfg;
    bool ok = cfg.loadFromFile("/nonexistent/config.txt");
    EXPECT_FALSE(ok);
    EXPECT_FALSE(cfg.extensions.empty());
    EXPECT_EQ(cfg.threadCount, 4u);
}

class IntegrationTest : public ParserTest {};

TEST_F(IntegrationTest, GivenEmptyDirectory_WhenScanned_ThenReturnsEmpty) {
    std::string emptyDir = testDir + "/emptySubDir";
    fs::create_directories(emptyDir);
    auto files = scanDirectory(emptyDir, {".cpp", ".h"});
    EXPECT_TRUE(files.empty());
}

TEST_F(IntegrationTest, GivenDirectoryWithFiles_WhenScanned_ThenFindsMatching) {
    std::string subDir = testDir + "/scanTest";
    fs::create_directories(subDir);
    std::ofstream(subDir + "/a.cpp") << "int x;\n";
    std::ofstream(subDir + "/b.h") << "int y;\n";
    std::ofstream(subDir + "/c.txt") << "text\n";
    auto files = scanDirectory(subDir, {".cpp", ".h"});
    EXPECT_EQ(files.size(), 2u);
}

TEST_F(IntegrationTest, GivenNoReadPermissions_WhenParsed_ThenReturnsEmptyStats) {
    std::string noReadPath = testDir + "/noread.cpp";
    std::ofstream(noReadPath) << "int x;\n";
    fs::permissions(noReadPath, fs::perms::none);
    auto stats = parseFile(noReadPath);
    EXPECT_EQ(stats.totalLines(), 0u);
    fs::permissions(noReadPath, fs::perms::owner_all);
}

TEST_F(IntegrationTest, GivenThreadPool_WhenTasksSubmitted_ThenTheyExecute) {
    threadPool pool;
    pool.initialize(2);
    std::atomic<int> counter{0};
    for (int i = 0; i < 10; ++i) {
        pool.add_task([&counter]() { counter.fetch_add(1); });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    pool.terminate();
    EXPECT_EQ(counter.load(), 10);
}
