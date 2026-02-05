#include "pch.h"

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <vector>

#include "../CodeLineCounter/FileProcessor.cpp"
#include "../CodeLineCounter/ReportWriter.cpp"
#include "../CodeLineCounter/ThreadPool.cpp"
#include "../CodeLineCounter/FileStat.h"

TEST(FileProcessorTests, EmptyFile)
{
    std::filesystem::path path = "empty.cpp";
    std::ofstream file(path);
    file.close();

    FileStat stat = FileProcessor::processFile(path);

    EXPECT_EQ(stat.blank_lines_count, 0);
    EXPECT_EQ(stat.comment_lines_count, 0);
    EXPECT_EQ(stat.code_lines_count, 0);

    std::filesystem::remove(path);
}

TEST(FileProcessorTests, BlankLines)
{
    std::filesystem::path path = "blank.cpp";
    std::ofstream file(path);
    file << "\n\n\n";
    file.close();

    FileStat stat = FileProcessor::processFile(path);

    EXPECT_EQ(stat.blank_lines_count, 3);
    EXPECT_EQ(stat.comment_lines_count, 0);
    EXPECT_EQ(stat.code_lines_count, 0);

    std::filesystem::remove(path);
}

TEST(FileProcessorTests, LineComments)
{
    std::filesystem::path path = "comments.cpp";
    std::ofstream file(path);
    file << "// comment\n"
        << "// comment\n";
    file.close();

    FileStat stat = FileProcessor::processFile(path);

    EXPECT_EQ(stat.comment_lines_count, 2);
    EXPECT_EQ(stat.code_lines_count, 0);
    EXPECT_EQ(stat.blank_lines_count, 0);

    std::filesystem::remove(path);
}

TEST(FileProcessorTests, BlockComments)
{
    std::filesystem::path path = "block.cpp";
    std::ofstream file(path);
    file << "/* comment\n"
        << "comment\n"
        << "*/\n";
    file.close();

    FileStat stat = FileProcessor::processFile(path);

    EXPECT_EQ(stat.comment_lines_count, 3);
    EXPECT_EQ(stat.code_lines_count, 0);

    std::filesystem::remove(path);
}

TEST(FileProcessorTests, MixedContent)
{
    std::filesystem::path path = "mixed.cpp";
    std::ofstream file(path);
    file << "\n"
        << "// comment\n"
        << "int a = 5;\n"
        << "/* block\n"
        << "comment */\n";
    file.close();

    FileStat stat = FileProcessor::processFile(path);

    EXPECT_EQ(stat.blank_lines_count, 1);
    EXPECT_EQ(stat.comment_lines_count, 3);
    EXPECT_EQ(stat.code_lines_count, 1);

    std::filesystem::remove(path);
}

TEST(ReportWriterTests, WritesOutputFile)
{
    FileStat stat;
    stat.name = "file.cpp";
    stat.path = "file.cpp";
    stat.code_lines_count = 1;
    stat.comment_lines_count = 2;
    stat.blank_lines_count = 3;
    stat.execution_time = 10;

    std::vector<FileStat> results = { stat };
    std::filesystem::path out = "test_report.txt";

    ReportWriter::writeToFile(results, out);

    std::ifstream file(out);
    ASSERT_TRUE(file.is_open());

    std::string content{ (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>() };

    EXPECT_FALSE(content.empty());

    file.close();
    std::filesystem::remove(out);
}

TEST(ThreadPoolTests, ProcessesSingleFile)
{
    std::filesystem::path path = "thread_test.cpp";
    std::ofstream file(path);
    file << "int a;\n";
    file.close();

    ThreadPool pool;
    pool.launchThreads();
    pool.addFilePath(path);
    pool.finishAdding();
    pool.joinThreads();

    auto results = pool.getResults();

    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].code_lines_count, 1);

    std::filesystem::remove(path);
}
