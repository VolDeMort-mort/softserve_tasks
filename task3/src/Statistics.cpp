#include "Statistics.h"

void TotalStats::add_file(const FileStats& f) {
    blank_lines += f.blank_lines;
    comment_lines += f.comment_lines;
    code_lines += f.code_lines;
    ++file_count;
}
