#include "ReadLiner.h"
#include <algorithm>
#include <fstream>

void Clock::start() {
    start_ = std::chrono::steady_clock::now();
}

std::chrono::microseconds Clock::stop() {
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
}

ReadLiner::ReadLiner(const std::filesystem::path& rootPath, unsigned char maxDepth)
    : root_(rootPath), maxDepth_(maxDepth) {}

void ReadLiner::setRoot(const std::filesystem::path& rootPath) {
    root_ = rootPath;
}

void ReadLiner::setMaxDepth(unsigned char maxDepth) {
    maxDepth_ = maxDepth;
}

bool ReadLiner::validateDirectory(const std::filesystem::path& path) {
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

Lines ReadLiner::processFile(const std::filesystem::path& filePath) {
    std::ifstream file(filePath);
    Lines lines{0, 0, 0, std::chrono::microseconds(0)};
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath.string());
    }

    std::string line;
    clock_->start();
    bool multiLineComment = false;
    while (std::getline(file, line)) {
        if (line.empty()) {
            lines.blank++;
        } else {
            // doesn`t work correctly when lines contains both code and comments
            auto it = std::find_if(line.begin(), line.end(), [](unsigned char c) { return !std::isspace(c); });
            if(multiLineComment && *it == '*' && *(it + 1) == '/') {
                multiLineComment = false;
                lines.comment++;
            } else if(multiLineComment) {
                lines.comment++;
            } else if(*it == '/' && *(it + 1) == '/') {
                lines.comment++;
            } else if(*it == '/' && *(it + 1) == '*') {
                multiLineComment = true;
                lines.comment++;
            } else {
                lines.code++;
            }
        }
    }
    lines.time_ms = clock_->stop();
    file.close();

    return lines;
}

Lines ReadLiner::processDirectory(const std::filesystem::path& dirPath, unsigned char currentDepth) {
    if (currentDepth > maxDepth_) {
        return Lines{0, 0, 0, std::chrono::microseconds(0)};
    }

    Lines lines{0, 0, 0, std::chrono::microseconds(0)};
    int fileCount = 0;

    results_.emplace_back(dirPath, Lines{0,0,0,std::chrono::microseconds(0)}, 0, currentDepth);
    int resultIndex = results_.size() - 1;

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.is_directory()) {
            lines += processDirectory(entry.path(), currentDepth + 1);
        } else if (entry.path().extension() == ".cpp" || entry.path().extension() == ".h" ||
                   entry.path().extension() == ".hpp" || entry.path().extension() == ".c") {
            lines += processFile(entry.path());
            fileCount++;
        }
    }

    results_[resultIndex] = std::make_tuple(dirPath, lines, fileCount, currentDepth);

    return lines;
}

void ReadLiner::run(const std::filesystem::path& pathToSaveFile) {
    if (!validateDirectory(root_)) {
        throw std::runtime_error("Invalid root directory: " + root_.string());
    }

    processDirectory(root_, 0);
    saveResults(pathToSaveFile);
}

void ReadLiner::saveResults(const std::filesystem::path& pathToSaveFile) {
    std::ofstream file(pathToSaveFile);
    if (!file) {
        throw std::runtime_error("Could not open file to save results: " + pathToSaveFile.string());
    }

    file << "Directory,Blank Lines,Comment Lines,Code Lines,Files Processed,Time (microseconds)\n";
    for (const auto& [dirPath, lines, fileCount, depth] : results_) {
        file << std::string(depth, '\t')
             << dirPath.string() << ","
             << lines.blank << ","
             << lines.comment << ","
             << lines.code << ","
             << fileCount << ","
             << lines.time_ms.count() << "\n";
    }
    file.close();
}