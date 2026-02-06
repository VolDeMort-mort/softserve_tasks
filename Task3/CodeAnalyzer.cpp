#include "CodeAnalyzer.h"
#include "FileUtils.h"
#include "ResultSaver.h"
#include <thread>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <iostream>

CodeAnalyzer::CodeAnalyzer(const std::string& rootPath) : rootPath(rootPath) {}

void CodeAnalyzer::run() {
    auto startTime = std::chrono::high_resolution_clock::now();
    sourceFiles = findSourceFiles(rootPath);

    if (sourceFiles.empty()) {
        std::cout << "No C/C++ source files found.\n";
        return;
    }

    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 2;
    numThreads = std::min(numThreads, static_cast<unsigned int>(sourceFiles.size()));

    processFiles(numThreads);

    std::sort(results.begin(), results.end(),
        [](const FileStatistics& a, const FileStatistics& b) { return a.filename < b.filename; });

    auto endTime = std::chrono::high_resolution_clock::now();
    double executionTime = std::chrono::duration<double>(endTime - startTime).count();

    printSummary(executionTime);
    saveResults("code_statistics.txt", globalStats, executionTime, results);
}

void CodeAnalyzer::workerThread(const std::vector<std::string>& files,
    size_t begin, size_t end,
    std::vector<FileStatistics>& results,
    GlobalStatistics& globalStats) {
    GlobalStatistics localStats{};
    std::vector<FileStatistics> localResults;

    for (size_t i = begin; i < end; ++i) {
        FileStatistics f;
        analyzeFile(files[i], f);
        localResults.push_back(f);
        localStats.files++;
        localStats.blank += f.blank;
        localStats.comment += f.comment;
        localStats.code += f.code;
    }

    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    results.insert(results.end(), localResults.begin(), localResults.end());
    globalStats.files += localStats.files;
    globalStats.blank += localStats.blank;
    globalStats.comment += localStats.comment;
    globalStats.code += localStats.code;
}

std::vector<std::pair<size_t, size_t>> CodeAnalyzer::splitWork(unsigned int numThreads) {
    std::vector<std::pair<size_t, size_t>> ranges;
    size_t filesPerThread = sourceFiles.size() / numThreads;
    size_t remaining = sourceFiles.size() % numThreads;
    size_t start = 0;

    for (unsigned int i = 0; i < numThreads; ++i) {
        size_t end = start + filesPerThread;
        if (i < remaining) end++;
        ranges.push_back({ start,end });
        start = end;
    }
    return ranges;
}

void CodeAnalyzer::processFiles(unsigned int numThreads) {
    auto ranges = splitWork(numThreads);
    std::vector<std::thread> threads;

    for (auto& r : ranges) {
        threads.emplace_back(workerThread,
            std::cref(sourceFiles),
            r.first,
            r.second,
            std::ref(results),
            std::ref(globalStats));
    }

    for (auto& t : threads) t.join();
}

void CodeAnalyzer::printSummary(double executionTime) {
    std::cout << "Total files:      " << globalStats.files << "\n";
    std::cout << "Blank lines:      " << globalStats.blank << "\n";
    std::cout << "Comment lines:    " << globalStats.comment << "\n";
    std::cout << "Code lines:       " << globalStats.code << "\n";
    std::cout << "Execution time:   " << executionTime << " seconds\n";
}
