#ifndef READLINER_H
#define READLINER_H

#define DEFAULT_SAVE_FILE "readliner.result.csv"
#define DEFAULT_DEPTH 3

#include <filesystem>
#include <string>
#include <chrono>
#include <vector>

struct Lines {
    int blank;
    int comment;
    int code;
    std::chrono::microseconds time_ms;

    Lines& operator+=(const Lines& other) {
        blank += other.blank;
        comment += other.comment;
        code += other.code;
        time_ms += other.time_ms;
        return *this;
    }
};

class Clock {
private:
    std::chrono::steady_clock::time_point start_;
public:
    void start();
    std::chrono::microseconds stop();
};

class ReadLiner {
private:
    std::filesystem::path root_;
    unsigned char maxDepth_;
    Clock *clock_ = new Clock();
    // directory, statistics, files processed, depth
    std::vector<std::tuple<std::filesystem::path, Lines, int, unsigned char>> results_;

    Lines processDirectory(const std::filesystem::path& dirPath, unsigned char currentDepth);
    Lines processFile(const std::filesystem::path& filePath);
    void saveResults(const std::filesystem::path& pathToSaveFile);
    bool validateDirectory(const std::filesystem::path& path);
public:
    ReadLiner(const std::filesystem::path& rootPath="", unsigned char maxDepth = DEFAULT_DEPTH);
    void setRoot(const std::filesystem::path& rootPath);
    void setMaxDepth(unsigned char maxDepth);
    void run(const std::filesystem::path& pathToSaveFile=DEFAULT_SAVE_FILE);
};


#endif // READLINER_H