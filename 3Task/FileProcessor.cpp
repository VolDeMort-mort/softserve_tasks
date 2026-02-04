#include "FileProcessor.h"
#include <fstream>
#include <string>
#include <cctype>

void FileProcessor::process_file(const fs::path& path, Counters& local) {
    std::ifstream file(path);
    if(!file) return;

    size_t local_empty = 0;
    size_t local_comment = 0;
    size_t local_code = 0;

    std::string line;
    bool in_block_comment = false;

    while(std::getline(file, line)) {
        size_t i = 0;
        bool has_code = false;
        bool has_comment = false;

        while(i < line.size()) {
            if(in_block_comment) {
                has_comment = true;
                if(i + 1 < line.size() && line[i] == '*' && line[i + 1] == '/') {
                    in_block_comment = false;
                    i += 2;
                } else {
                    ++i;
                }
                continue;
            }

            if(std::isspace(line[i])) {
                ++i;
                continue;
            }

            if(i + 1 < line.size() && line[i] == '/' && line[i + 1] == '/') {
                has_comment = true;
                break;
            }

            if(i + 1 < line.size() && line[i] == '/' && line[i + 1] == '*') {
                has_comment = true;
                in_block_comment = true;
                i += 2;
                continue;
            }

            has_code = true;
            ++i;
        }

        if(!has_code && !has_comment)
            ++local_empty;
        else if(has_comment && !has_code)
            ++local_comment;
        else
            ++local_code;
    }

    local.empty += local_empty;
    local.comment += local_comment;
    local.code += local_code;
    local.files += 1;
}

