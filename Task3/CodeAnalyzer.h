#pragma once

#include "FileAnalyzer.h"

#include <string>
#include <vector>

class CodeAnalyzer {
public:
	CodeAnalyzer(const std::string& rootPath);
	void run();

private:
	std::string rootPath;
	std::vector<std::string>sourceFiles;
	std::vector<FileStatistics> results;
	GlobalStatistics globalStats;

	void processFiles(unsigned int numThreads);
	void printSummary(double time);
	std::vector<std::pair<size_t, size_t>> splitWork(unsigned int numThreads);

	static void workerThread(const std::vector<std::string>& files, size_t begin, size_t end,
		std::vector<FileStatistics>& results, GlobalStatistics& globalStats);
};