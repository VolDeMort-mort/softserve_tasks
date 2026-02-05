#pragma once
#include <vector>
#include "FileStat.h"
#include <string>
class ReportWriter{
public:
	static void writeToFile(const std::vector<FileStat>& results, const std::filesystem::path& path);
};

