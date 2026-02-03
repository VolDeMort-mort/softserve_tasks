#ifndef LINE_COUNTER_H
#define LINE_COUNTER_H

#include <string>

struct FileStats {
    std::string path;
    int blank_lines   = 0;
    int comment_lines = 0;
    int code_lines    = 0;
};

FileStats count_lines_in_file(const std::string& filepath);

#endif
