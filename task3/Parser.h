#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <string>
#include <vector>

struct FileStats {
    std::string path;
    std::uint32_t blankLines = 0;
    std::uint32_t commentLines = 0;
    std::uint32_t codeLines = 0;

    std::uint32_t totalLines() const {
        return blankLines + commentLines + codeLines;
    }
};

bool isBlankLine(const std::string& line);
bool isBinaryFile(const std::string& filePath);
FileStats parseFile(const std::string& filePath);
std::vector<std::string> scanDirectory(const std::string& dirPath, const std::vector<std::string>& extensions);

#endif
