#include "pch.h"

#include "../task3_lib/ProjectScanner.h"
#include "../task3_lib/FileInfoWriter.h"
#include "../task3_lib/ProjectResult.h"
#include "../task3_lib/FileResult.h"

class TestAnalyzeFile : public ::testing::TestWithParam<std::pair<std::string, std::tuple<unsigned int, unsigned int, unsigned int>>> {
};

TEST_P(TestAnalyzeFile, TestAnalyseSeparateFilesParam) {
    auto param = GetParam();
    std::filesystem::path file = param.first;
    auto expected = param.second;
    auto res = ProjectScanner::analyzeFile(file);
    EXPECT_TRUE(res.opened);
    EXPECT_EQ(res.code, std::get<0>(expected));
    EXPECT_EQ(res.comments, std::get<1>(expected));
    EXPECT_EQ(res.blanks, std::get<2>(expected));
}


INSTANTIATE_TEST_CASE_P(
    AnalizeFileParametrs,
    TestAnalyzeFile,
    ::testing::Values(
        std::make_pair(std::string("C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir1\\0code0comm5bl.cpp"), std::make_tuple(0u, 0u, 5u)),
        std::make_pair(std::string("C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir1\\0code2comm1bl.cpp"), std::make_tuple(0u, 2u, 1u)),
        std::make_pair(std::string("C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir1\\2code4comm2bl.cpp"), std::make_tuple(2u, 4u, 2u)),
        std::make_pair(std::string("C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir1\\3code4comm1bl.cpp"), std::make_tuple(3u, 4u, 1u)),
        std::make_pair(std::string("C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir1\\5code0comm0bl.cpp"), std::make_tuple(5u, 0u, 0u)),
        std::make_pair(std::string("C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir1\\6code0comm1bl.cpp"), std::make_tuple(6u, 0u, 1u)),
        std::make_pair(std::string("C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir1\\6code0comm1bl.h"), std::make_tuple(6u, 0u, 1u)),
        std::make_pair(std::string("C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir1\\7code6comm1bl.cpp"), std::make_tuple(7u, 6u, 1u)),
        std::make_pair(std::string("C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir2_single_file\\1code1comm1bl.c"), std::make_tuple(1u, 1u, 1u))
    )
);

class TestWriter : public AbstractResultWriter {
public:
    ProjectResult m_prResult;

    TestWriter() = default;

    void write(const ProjectResult& prResult) override {
        m_prResult = prResult;
    }
};

static const std::filesystem::path dir1 = "C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir1";
static const std::filesystem::path dir2 = "C:\\dev\\softserve_intership\\task3\\UnitTests\\test_dirs\\dir2_single_file";
static const std::filesystem::path dir3 = "ddd";

class TestProjectScanner : public ::testing::Test {
protected:
    std::unique_ptr<ProjectScanner> prScanner;
    TestWriter* writerPtr = nullptr;

    void SetUp() override {
        std::unique_ptr<TestWriter> writer = std::make_unique<TestWriter>();
        writerPtr = writer.get();
        prScanner = std::make_unique<ProjectScanner>(dir1, std::move(writer));
    }

    void TearDown() override {}
};

TEST_F(TestProjectScanner, TestDir1) {
    prScanner->proccess();
    
    uint64_t totalBlanks = 0;
    uint64_t totalCode = 0;
    uint64_t totalComments = 0;

    const ProjectResult& prResult = writerPtr->m_prResult;
    size_t size = prResult.m_filesResults.size();
    const std::vector<FileResult>& filesResults(prResult.m_filesResults);

    for (int i = 0; i < size; ++i) {
        if (filesResults[i].opened) {
            totalBlanks += filesResults[i].blanks;
            totalComments += filesResults[i].comments;
            totalCode += filesResults[i].code;
        }
    }

    EXPECT_EQ(totalCode, 29);
    EXPECT_EQ(totalComments, 16);
    EXPECT_EQ(totalBlanks, 12);

}

TEST_F(TestProjectScanner, TestDir2) {
    prScanner->changeDirectory(dir2);

    prScanner->proccess();

    uint64_t totalBlanks = 0;
    uint64_t totalCode = 0;
    uint64_t totalComments = 0;

    const ProjectResult& prResult = writerPtr->m_prResult;
    size_t size = prResult.m_filesResults.size();
    const std::vector<FileResult>& filesResults(prResult.m_filesResults);

    for (int i = 0; i < size; ++i) {
        if (filesResults[i].opened) {
            totalBlanks += filesResults[i].blanks;
            totalComments += filesResults[i].comments;
            totalCode += filesResults[i].code;
        }
    }

    EXPECT_EQ(totalCode, 1);
    EXPECT_EQ(totalBlanks, 1);
    EXPECT_EQ(totalComments, 1);

}

TEST_F(TestProjectScanner, TestDir3) {
    prScanner->changeDirectory(dir3);

    prScanner->proccess();

    uint64_t totalBlanks = 0;
    uint64_t totalCode = 0;
    uint64_t totalComments = 0;

    const ProjectResult& prResult = writerPtr->m_prResult;
    size_t size = prResult.m_filesResults.size();
    const std::vector<FileResult>& filesResults(prResult.m_filesResults);

    for (int i = 0; i < size; ++i) {
        if (filesResults[i].opened) {
            totalBlanks += filesResults[i].blanks;
            totalComments += filesResults[i].comments;
            totalCode += filesResults[i].code;
        }
    }

    EXPECT_EQ(totalCode, 0);
    EXPECT_EQ(totalBlanks, 0);
    EXPECT_EQ(totalComments, 0);

}

TEST(TestFileInfoWriter, TestWrite) {
    std::filesystem::path outFile = "C:\\dev\\softserve_intership\\task3\\UnitTests\\result.txt";
    ProjectScanner prScanner(dir1, std::make_unique<FileInfoWriter>(outFile, true));
    prScanner.proccess();

    ASSERT_TRUE(std::filesystem::exists(outFile));

    std::ifstream fin(outFile, std::ios::in);
    ASSERT_TRUE(fin.is_open());

    std::string firstLine;
    std::getline(fin, firstLine);
    EXPECT_FALSE(firstLine.empty());
}