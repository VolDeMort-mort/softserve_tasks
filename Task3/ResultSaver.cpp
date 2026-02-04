#include "ResultSaver.h"
#include <fstream>
#include <iostream>

void saveResults(const std::string& outputFile,
    const GlobalStatistics& stats,
    double executionTime,
    const std::vector<FileStatistics>& fileResults) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Error: Could not create output file: " << outputFile << std::endl;
        return;
    }

    out << "========================================\n";
    out << "   Source Code Statistics Report\n";
    out << "========================================\n\n";

    out << "Summary:\n";
    out << "Total files:      " << stats.files << "\n";
    out << "Blank lines:      " << stats.blank << "\n";
    out << "Comment lines:    " << stats.comment << "\n";
    out << "Code lines:       " << stats.code << "\n";
    out << "Execution time:   " << executionTime << " seconds\n\n";

    out << "Detailed File Statistics:\n";
    for (const auto& f : fileResults) {
        out << "\nFile: " << f.filename << "\n";
        out << "  Blank lines:   " << f.blank << "\n";
        out << "  Comment lines: " << f.comment << "\n";
        out << "  Code lines:    " << f.code << "\n";
    }

    out.close();
}
