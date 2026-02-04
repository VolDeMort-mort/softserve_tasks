#include "FileAnalyzer.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cctype>

bool isBlank(const std::string& line) {
	return std::all_of(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c); });
}

void analyzeFile(const std::string& path, FileStatistics& stats){
	stats.filename = path;
	std::ifstream file(path);

	if (!file) {
		std::cout << "Error: Couldnt open file on: " << path << std::endl;
		return;
	}

	bool inCommentBlock = false;
	std::string line;

	while (std::getline(file, line)) {
		if (isBlank(line)) { stats.blank++; continue; }

		bool hasCode = false;
		bool hasComment = false;
		bool inString = false;
		bool inChar = false;
		bool isSpecial = false;

		for (size_t i = 0; i < line.size(); ++i) {
			char c = line[i];
			char next = (i + 1 < line.size()) ? line[i + 1] : '\0';

			if (isSpecial) { isSpecial = false; hasCode = true; continue; }
			if ((inString || inChar) && c == '\\') { isSpecial = true; hasCode = true; continue; }

			if (!inChar && c == '"' && !inCommentBlock) { inString = !inString; hasCode = true; continue; }
			if (!inString && c == '\'' && !inCommentBlock) { inChar = !inChar; hasCode = true; continue; }

			if (inString || inChar) { hasCode = true; continue;}

			if (!inCommentBlock && c == '/' && next == '/') { hasComment = true; break;}
			if (!inCommentBlock && c == '/' && next == '*') { hasComment = true; inCommentBlock = true; ++i; continue; }
			if (inCommentBlock && c == '*' && next == '/') { inCommentBlock = false; ++i; continue; }

			if (!inCommentBlock && !std::isspace(static_cast<unsigned char>(c))) { hasCode = true; }
		}

		if (hasCode) stats.code++;
		else if (hasComment || inCommentBlock ) stats.comment++;
		else stats.code++;
	}
}