#pragma once
#include <string>
#include "Statistics.h"

class FileAnalyzer {
public:
    static FileStats analyze(const std::string& path);
};
