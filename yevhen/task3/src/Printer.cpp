#include "Printer.h"
#include <fstream>

void CSVPrinter::save(const std::filesystem::path& pathToSaveFile, 
                    const std::vector<std::tuple<std::filesystem::path, Lines, int, unsigned char>> &results) {
    std::ofstream file(pathToSaveFile);
    if (!file) {
        throw std::runtime_error("Could not open file to save results: " + pathToSaveFile.string());
    }

    file << "Path,Blank Lines,Comment Lines,Code Lines,Files Processed,Time (microseconds)\n";
    for (const auto& [dirPath, lines, fileCount, depth] : results) {
        file << dirPath.generic_string() << ","
             << lines.blank << ","
             << lines.comment << ","
             << lines.code << ","
             << fileCount << ","
             << lines.time_ms.count() << "\n";
    }
    file.close();
}

void PrettyPrinter::save(const std::filesystem::path& pathToSaveFile, 
                        const std::vector<std::tuple<std::filesystem::path, Lines, int, unsigned char>> &results) {
    std::ofstream file(pathToSaveFile);
    if (!file) {
        throw std::runtime_error("Could not open file to save results: " + pathToSaveFile.string());
    }

    file << "Name - Blank Lines - Comment Lines - Code Lines - Files Processed - Time (microseconds)\n";
    for (const auto& [dirPath, lines, fileCount, depth] : results) {
        file << std::string(depth, '\t')
             << dirPath.filename().generic_string() << " - "
             << lines.blank << " - "
             << lines.comment << " - "
             << lines.code << " - "
             << fileCount << " - "
             << lines.time_ms.count() << "\n";
    }
    file.close();
}