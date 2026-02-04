#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "FileCollector.h"
#include "ThreadPool.h"
#include "Counters.h"

namespace fs = std::filesystem;

static fs::path create_test_project() {
    fs::path root = fs::temp_directory_path() / "cpp_counter_test";
    fs::create_directories(root / "sub");

    {
        std::ofstream f(root / "file1.cpp");
        f <<
            "int a = 0;\n"
            "// comment\n"
            "\n";
    }

    {
        std::ofstream f(root / "sub" / "file2.h");
        f <<
            "/* start\n"
            "middle\n"
            "end */\n"
            "int b;\n";
    }

    {
        std::ofstream f(root / "file3.txt");
        f << "should be ignored\n";
    }

    return root;
}

TEST(ApplicationIntegration, CountsWholeProjectCorrectly) {
    fs::path root = create_test_project();

    auto files = FileCollector::collect(root);
    Counters result = ThreadPool::run(files);

    // expect:
    // file1.cpp:
    //   code: 1
    //   comment: 1
    //   empty: 1
    //
    // file2.h:
    //   comment: 3
    //   code: 1

    EXPECT_EQ(result.files, 2);
    EXPECT_EQ(result.empty, 1);
    EXPECT_EQ(result.comment, 4);
    EXPECT_EQ(result.code, 2);
}

