#include "FileAnalyzer.h"
#include <fstream>
#include <string>
#include <algorithm>

static std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

FileStats FileAnalyzer::analyze(const std::string& path) {
    FileStats stats;
    std::ifstream in(path);
    if (!in.is_open()) {
        return stats;
    }

    std::string line;
    bool inBlockComment = false;

    while (std::getline(in, line)) {
        std::string t = trim(line);

        if (t.empty()) {
            ++stats.blank_lines;
            continue;
        }

        if (inBlockComment) {
            ++stats.comment_lines;
            if (t.find("*/") != std::string::npos)
                inBlockComment = false;
            continue;
        }

        if (t.rfind("/*", 0) == 0) {
            ++stats.comment_lines;
            if (t.find("*/") == std::string::npos)
                inBlockComment = true;
            continue;
        }

        if (t.rfind("//", 0) == 0) {
            ++stats.comment_lines;
        }
        else {
            ++stats.code_lines;
        }
    }

    return stats;
}
