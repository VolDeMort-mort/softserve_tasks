#include "line_counter.h"
#include <fstream>
#include <stdexcept>

FileStats count_lines_in_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filepath);

    FileStats stats;
    stats.path = filepath;

    bool in_block_comment = false;
    std::string line;

    while (std::getline(file, line)) {
        if (!in_block_comment && line.find_first_not_of(" \t\r") == std::string::npos) {
            ++stats.blank_lines;
            continue;
        }

        bool has_code = false;
        bool has_comment = false;
        bool in_string = false;
        bool in_char = false;

        for (auto it = line.begin(); it != line.end(); ++it) {
            auto c = *it;
            auto next = (std::next(it) != line.end()) ? *std::next(it) : '\0';

            if (in_block_comment) {
                has_comment = true;
                if (c == '*' && next == '/') {
                    in_block_comment = false;
                    ++it;
                }
                continue;
            }

            if (in_string) {
                has_code = true;
                if (c == '\\') {
                    ++it;
                } else if (c == '"') {
                    in_string = false;
                }
                continue;
            }

            if (in_char) {
                has_code = true;
                if (c == '\\') {
                    ++it;
                } else if (c == '\'') {
                    in_char = false;
                }
                continue;
            }

            if (c == '/' && next == '/') {
                has_comment = true;
                break;
            }

            if (c == '/' && next == '*') {
                has_comment = true;
                in_block_comment = true;
                ++it;
                continue;
            }

            if (c == '"') {
                in_string = true;
                has_code = true;
                continue;
            }

            if (c == '\'') {
                in_char = true;
                has_code = true;
                continue;
            }

            if (c != ' ' && c != '\t' && c != '\r') {
                has_code = true;
            }
        }

        if (has_code)    ++stats.code_lines;
        if (has_comment) ++stats.comment_lines;
        if (!has_code && !has_comment) ++stats.blank_lines;
    }

    return stats;
}
