#include <iostream>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "FileScanner.h"
#include "FileAnalyzer.h"
#include "Statistics.h"
#include "ReportWriter.h"


std::queue<std::string> g_tasks;
std::mutex g_mutex;
std::mutex g_stats_mutex;
std::condition_variable g_cv;
bool g_done = false;


std::string get_root_path(int argc, char* argv[]) {
    if (argc >= 2) {
        return argv[1];
    }
    return "C:\\projects\\trash\\cpp_test"; 
}

void worker(TotalStats& total) {
    while (true) {
        std::string path;
        {
            std::unique_lock<std::mutex> lock(g_mutex);
            g_cv.wait(lock, [] { return !g_tasks.empty() || g_done; });

            if (g_tasks.empty() && g_done)
                return;

            path = g_tasks.front();
            g_tasks.pop();
        }

        FileStats fs = FileAnalyzer::analyze(path);
        {
            std::lock_guard<std::mutex> lock(g_stats_mutex);
            total.add_file(fs);
        }

    }
}


TotalStats analyze_project(const std::string& root) {
    FileScanner scanner(root);
    scanner.scan();
    const auto& files = scanner.getFiles();

    TotalStats total;

    {
        std::lock_guard<std::mutex> lock(g_mutex);
        for (const auto& p : files) {
            g_tasks.push(p.string());
        }
    }

    unsigned int threads_count = std::thread::hardware_concurrency();
    if (threads_count == 0) threads_count = 4;

    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < threads_count; ++i) {
        threads.emplace_back(worker, std::ref(total));
    }

    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_done = true;
    }
    g_cv.notify_all();

    for (auto& t : threads) {
        t.join();
    }

    return total;
}


int main() {
    while (true) {
        std::cout << "1) Analyze folder\n"
            << "0) Exit\n"
            << "Choice: ";
        int choice{};
        std::cin >> choice;

        if (choice == 0) break;
        if (choice != 1) continue;

        std::cout << "Enter root folder path: ";
        std::string root;
        std::cin >> root;

        auto start = std::chrono::high_resolution_clock::now();
        TotalStats total = analyze_project(root);
        auto end = std::chrono::high_resolution_clock::now();

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        total.execution_time = static_cast<double>(ms);

        std::cout << "Files: " << total.file_count << "\n"
            << "Blank: " << total.blank_lines << "\n"
            << "Comments: " << total.comment_lines << "\n"
            << "Code: " << total.code_lines << "\n"
            << "Time: " << total.execution_time << " ms\n";

        ReportWriter::write("results.txt", total);
    }
}
