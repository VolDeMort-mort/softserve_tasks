#pragma once

#include "FileAnalyzer.h"

#include <vector>
#include <string>

void saveResults(const std::string& outputFile,
    const GlobalStatistics& stats,
    double executionTime,
    const std::vector<FileStatistics>& fileResults);