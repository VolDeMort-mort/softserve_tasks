#include "Config.h"
#include "Parser.h"
#include "ThreadPool.h"
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace {
    std::atomic<bool> interrupted{false};
}

void signalHandler(int signum) {
    if (signum == SIGINT) {
        interrupted.store(true);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "How to use: main <directory> <config.txt>\n";
        return 1;
    }
    std::string targetDir = argv[1];
    std::string configPath = (argc >= 3) ? argv[2] : "config.txt";

    Config config;
    config.loadFromFile(configPath);

    auto startTime = std::chrono::steady_clock::now();
    std::vector<std::string> files = scanDirectory(targetDir, config.extensions);

    if (files.empty()) {
        std::cout << "No matching files found in: " << targetDir << "\n";
        return 0;
    }

    std::cout << "Found " << files.size() << " file(s), processing with " << config.threadCount << " thread(s)...\n\n";

    std::vector<FileStats> results(files.size());
    std::atomic<std::size_t> filesCompleted{0};

    threadPool pool;
    pool.initialize(static_cast<int>(config.threadCount));

    for (size_t i = 0; i < files.size(); ++i) {
        pool.add_task([&results, &files, i, &filesCompleted]() {
            results[i] = parseFile(files[i]);
            filesCompleted.fetch_add(1, std::memory_order_relaxed);
            filesCompleted.notify_all();
        });
    }

    std::signal(SIGINT, signalHandler);

    while (true) {
        size_t completed = filesCompleted.load(std::memory_order_relaxed);
        if (completed >= files.size() || interrupted.load()) {
            break;
        }
        filesCompleted.wait(completed, std::memory_order_relaxed);
    }

    if (interrupted.load()) {
        std::cout << "\nInterrupted, but I will show partial results.\n";
    }

    pool.terminate();

    auto endTime = std::chrono::steady_clock::now();
    auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::uint32_t totalBlank = 0, totalComment = 0, totalCode = 0;
    size_t processedCount = 0;

    for (const auto& fs : results) {
        if (fs.totalLines() > 0) {
            totalBlank += fs.blankLines;
            totalComment += fs.commentLines;
            totalCode += fs.codeLines;
            processedCount++;
        }
    }

    std::cout << std::left << std::setw(30) << "File" << std::right << std::setw(8) << "blank" << std::setw(10) << "comment" << std::setw(8) << "code" << "\n";

    for (const auto& fs : results) {
        if (fs.totalLines() > 0) {
            std::cout << std::left << std::setw(30) << fs.path << std::right << std::setw(8) << fs.blankLines << std::setw(10) << fs.commentLines << std::setw(8) << fs.codeLines << "\n";
        }
    }
    std::cout << std::left << std::setw(30) << "total" << std::right << std::setw(8) << totalBlank << std::setw(10) << totalComment << std::setw(8) << totalCode << "\n\n";
    std::cout << "Scanned " << processedCount << " of " << files.size() << " files, took " << elapsedMs << " ms\n";

    std::ofstream outFile(config.outputFilePath);
    if (outFile.is_open()) {
        outFile << "~~~results~~~\n\n";
        for (const auto& fs : results) {
            if (fs.totalLines() > 0) {
                 outFile << "the path " << fs.path << "\n";
                std::uint32_t total = fs.totalLines();
                outFile << "blank lines = " << fs.blankLines << "\n";
                outFile << "comment lines = " << fs.commentLines << "\n";
                outFile << "code lines = " << fs.codeLines << "\n";
                outFile << "total lines = " << total << "\n\n";
            }
        }
        outFile << "The summary\n";
        outFile << "total blank lines = " << totalBlank << "\n";
        outFile << "total comment lines = " << totalComment << "\n";
        outFile << "total code lines = " << totalCode << "\n";
        outFile << "total files scanned = " << processedCount << "\n";
        outFile << "elapsed time = " << elapsedMs << " ms\n";
        std::cout << "Results saved to " << config.outputFilePath << "\n";
    } else {
        std::cerr << "Could not write to " << config.outputFilePath << "\n";
    }

    return 0;
}
