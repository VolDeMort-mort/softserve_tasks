#pragma once

#include <string>

struct FileStatistics {
	std::string filename;
	int blank = 0;
	int comment = 0;
	int code = 0;
};

struct GlobalStatistics {
	int files = 0;
	int blank = 0;
	int comment = 0;
	int code = 0;
};

void analyzeFile(const std::string& path, FileStatistics& stats);

bool isBlank(const std::string& line);