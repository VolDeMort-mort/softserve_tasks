#include "ReportWriter.h"
#include <fstream>
#include <iomanip>

void ReportWriter::write(const std::string& file_path, const TotalStats& stats) {
    std::ofstream out(file_path);
    if (!out.is_open()) {
        return;
    }

    out << "Files: " << stats.file_count << '\n'
        << "Blank: " << stats.blank_lines << '\n'
        << "Comments: " << stats.comment_lines << '\n'
        << "Code: " << stats.code_lines << '\n'
        << "Time: " << stats.execution_time << " ms\n";
}
