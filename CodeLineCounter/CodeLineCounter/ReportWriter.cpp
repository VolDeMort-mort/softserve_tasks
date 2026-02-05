#include "ReportWriter.h"
#include <fstream>

void ReportWriter::writeToFile(const std::vector<FileStat>& results, const std::filesystem::path& path){
	std::ofstream file(path);
    if (!file) {
        return;
    }

    for (const auto& stat : results) {
        file << "Path to file: " << stat.path << "\n"
            << "File name: " << stat.name << "\n"
            << "Number of code lines: " << stat.code_lines_count << "\n"
            << "Number of comment lines: " << stat.comment_lines_count << "\n"
            << "Number of blank lines: " << stat.blank_lines_count << "\n"
            << "Time of execution: " << stat.execution_time << " ms\n\n";
    }
}
