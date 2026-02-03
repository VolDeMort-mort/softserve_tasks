#include "line_counter.h"

#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

static const std::vector<std::string> extensions = {".h", ".hpp", ".c", ".cpp"};

static bool is_source_file(const fs::path& path) {
    auto ext = path.extension().string();
    for (const auto& e : extensions)
        if (ext == e) return true;
    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: line_counter <project_root>\n";
        return 1;
    }

    auto root = fs::path(argv[1]);
    if (!fs::is_directory(root)) {
        std::cerr << "Error: " << root << " is not a directory\n";
        return 1;
    }

    auto start_time = std::chrono::steady_clock::now();

    std::queue<std::string> work_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
    bool done = false;

    std::vector<FileStats> results;
    std::mutex results_mutex;

    auto num_workers = std::max(2u, std::thread::hardware_concurrency());

    auto worker = [&] {
        while (true) {
            std::string filepath;
            {
                std::unique_lock lock(queue_mutex);
                queue_cv.wait(lock, [&] { return !work_queue.empty() || done; });
                if (work_queue.empty() && done)
                    break;
                filepath = std::move(work_queue.front());
                work_queue.pop();
            }

            try {
                auto stats = count_lines_in_file(filepath);
                std::lock_guard lock(results_mutex);
                results.push_back(std::move(stats));
            } catch (const std::exception& e) {
                std::lock_guard lock(results_mutex);
                std::cerr << "Warning: " << e.what() << "\n";
            }
        }
    };

    std::vector<std::thread> threads;
    for (unsigned i = 0; i < num_workers; ++i)
        threads.emplace_back(worker);

    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (entry.is_regular_file() && is_source_file(entry.path())) {
            std::lock_guard lock(queue_mutex);
            work_queue.push(entry.path().string());
            queue_cv.notify_one();
        }
    }

    {
        std::lock_guard lock(queue_mutex);
        done = true;
    }
    queue_cv.notify_all();

    for (auto& t : threads)
        t.join();

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<double>(end_time - start_time);

    int total_blank = 0, total_comment = 0, total_code = 0;
    for (const auto& s : results) {
        total_blank   += s.blank_lines;
        total_comment += s.comment_lines;
        total_code    += s.code_lines;
    }

    std::cout << "Files processed: " << results.size() << "\n"
              << "Blank lines:     " << total_blank << "\n"
              << "Comment lines:   " << total_comment << "\n"
              << "Code lines:      " << total_code << "\n"
              << "Execution time:  " << elapsed.count() << "s\n";

    return 0;
}
