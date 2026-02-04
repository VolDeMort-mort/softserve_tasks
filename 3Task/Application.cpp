#include "Application.h"
#include "FileCollector.h"
#include "ThreadPool.h"

#include <chrono>
#include <fstream>
#include <iostream>

int Application::run(const std::filesystem::path& root) {
	auto start = std::chrono::high_resolution_clock::now();

    auto files = FileCollector::collect(root);
    Counters result = ThreadPool::run(files);

	auto end = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::ofstream out("result.txt");
    out << "Files: " << result.files << '\n';
    out << "Blank lines: " << result.empty << '\n';
    out << "Comment lines: " << result.comment << '\n';
    out << "Code lines: " << result.code << '\n';
    out << "Execution time (ms): " << time << '\n';

    std::cout << out.rdbuf();

    return 0;
}

