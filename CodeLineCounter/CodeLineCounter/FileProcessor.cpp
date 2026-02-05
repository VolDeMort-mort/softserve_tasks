#include "FileProcessor.h"
#include <fstream>
#include <string>
#include <iostream>

FileStat FileProcessor::processFile(const std::filesystem::path& path)
{
    FileStat fileStat;
    fileStat.name = path.filename().string();
    fileStat.path = path;

    std::ifstream file(path);
    std::string line;
    if (file.is_open()) {
        auto start = std::chrono::high_resolution_clock::now();
        bool in_block_comment = false;
        while (std::getline(file, line)) {
            trim(line);

            if (line.empty()) {
                ++fileStat.blank_lines_count;
                continue; 
            }

            if (in_block_comment) {
                ++fileStat.comment_lines_count;
                if (line.find("*/") != std::string::npos) {
                    in_block_comment = false;
                }
                continue; 
            }

            if (line.size() >= 2 && line.substr(0, 2) == "//") {
                ++fileStat.comment_lines_count;
                continue;
            }

            if (line.size() >= 2 && line.substr(0, 2) == "/*") {
                ++fileStat.comment_lines_count;
                if (line.find("*/") == std::string::npos) {
                    in_block_comment = true;
                }
                continue;
            }

            ++fileStat.code_lines_count; 
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        fileStat.execution_time = duration.count();
    }
    return fileStat;
}

void FileProcessor::trim(std::string& s) {
    auto first = s.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) {
        s.clear();
        return;
    }
    auto last = s.find_last_not_of(" \t\n\r");
    s = s.substr(first, last - first + 1);
}