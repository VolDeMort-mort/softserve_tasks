#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "FileAnalyzer.h"

namespace fs = std::filesystem;

TEST(SafeQueueTest, PushAndPop) {
     SafeQueue<int> q;
     q.push(10);
     q.push(20);

     int val;
     EXPECT_TRUE(q.pop(val));
     EXPECT_EQ(val, 10);
     EXPECT_TRUE(q.pop(val));
     EXPECT_EQ(val, 20);
}

TEST(LineCounterTest, BasicCounting) {
     std::string root_path = "testFolder";

     total_files = 0;
     total_blank = 0;
     total_comment = 0;
     total_code = 0;

     SafeQueue<std::string> file_queue;
     unsigned int num_threads = std::thread::hardware_concurrency();
     std::vector<std::thread> workers;
     for (unsigned int i = 0; i < num_threads; ++i) {
          workers.emplace_back([&file_queue]() {
               std::string path;
               while (file_queue.pop(path)) {
                    analyze_file(path);
               }
               });
     }

     for (const auto& entry : fs::recursive_directory_iterator(root_path)) {
          if (entry.is_regular_file()) {
               std::string ext = entry.path().extension().string();
               if (ext == ".h" || ext == ".hpp" || ext == ".c" || ext == ".cpp") {
                    file_queue.push(entry.path().string());
               }
          }
     }
     file_queue.set_finished();

     for (auto& t : workers) t.join();

     EXPECT_EQ(total_blank, 7);
     EXPECT_EQ(total_comment, 0);
     EXPECT_EQ(total_code, 38);

}

int main(int argc, char** argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}