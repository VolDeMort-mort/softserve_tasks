#ifndef READLINER_H
#define READLINER_H

#define DEFAULT_SAVE_FILE "readliner_result.csv"
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

    Lines& operator+=(const Lines& other);
};

class Clock {
private:
    std::chrono::steady_clock::time_point start_;
public:
    void start();
    std::chrono::microseconds stop();
};

enum class SaveMode{
    CSV,
    Pretty,
};

using Result = std::tuple<
    std::filesystem::path,
    Lines,
    int,
    unsigned char
>;

struct DirResult {
    Lines lines;
    std::vector<Result> results;
};

class ReadLiner {
private:
    std::filesystem::path root_;
    unsigned char maxDepth_;
    Clock *clock_ = new Clock();
    // directory, statistics, files processed, depth
    std::vector<std::tuple<std::filesystem::path, Lines, int, unsigned char>> results_;

    DirResult processDirectory(const std::filesystem::path& dirPath, unsigned char currentDepth);
    Lines processFile(const std::filesystem::path& filePath);
    bool validateDirectory(const std::filesystem::path& path);
public:
    ReadLiner(const std::filesystem::path& rootPath="", unsigned char maxDepth = DEFAULT_DEPTH);
    void setRoot(const std::filesystem::path& rootPath);
    void setMaxDepth(unsigned char maxDepth);
    void run(const std::filesystem::path& pathToSaveFile=DEFAULT_SAVE_FILE, SaveMode mode=SaveMode::CSV);
};


#endif // READLINER_H