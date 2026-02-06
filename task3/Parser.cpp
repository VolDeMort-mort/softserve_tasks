#include "Parser.h"
#include <algorithm>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

bool isBlankLine(const std::string& line) {
    return line.find_first_not_of(" \t\r\n") == std::string::npos;
}

FileStats parseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return {};

    FileStats stats;
    stats.path = filePath;
    
    std::string line;
    bool inBlockComment = false;

    while (std::getline(file, line)) {
        if (line.empty()) {
            stats.blankLines++;
            continue;
        }

        bool hasCode = false, hasComment = false, isBlank = true; 

        size_t i = 0;
        const size_t len = line.length();

        while (i < len) {
            if (inBlockComment) {
                size_t endPos = line.find("*/", i);
                if (endPos != std::string::npos) {
                    inBlockComment = false;
                    hasComment = true;
                    i = endPos + 2; 
                } else {
                    hasComment = true;
                    break;
                }
                continue;
            }

            char c = line[i];

            if (std::isspace(c)) {
                i++;
                continue;
            }
            isBlank = false;

            if (c == '/' && i + 1 < len) {
                if (line[i+1] == '/') {
                    hasComment = true;
                    break;
                }
                if (line[i+1] == '*') {
                    hasComment = true;
                    inBlockComment = true;
                    i += 2;
                    continue;
                }
            }

            if (c == '"' || c == '\'') {
                hasCode = true;
                char quote = c;
                i++;
                while (i < len) {
                    if (line[i] == '\\') i += 2;
                    else if (line[i] == quote) { i++; break; }
                    else i++;
                }
                continue;
            }
            hasCode = true;
            i++;
        }

        if (isBlank) {
            stats.blankLines++;
        } else if (hasCode) {
            stats.codeLines++;
        } else if (hasComment) {
            stats.commentLines++;
        } else {
            stats.blankLines++;
        }
    }

    return stats;
}

std::vector<std::string> scanDirectory(const std::string& dirPath, const std::vector<std::string>& extensions) {
    std::vector<std::string> files;

    std::error_code ec;
    for (const auto& entry : fs::recursive_directory_iterator(dirPath, ec)) {
        if (ec) break;
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        for (const auto& allowedExt : extensions) {
            if (ext == allowedExt) {
                files.push_back(entry.path().string());
                break;
            }
        }
    }

    std::sort(files.begin(), files.end());
    return files;
}
