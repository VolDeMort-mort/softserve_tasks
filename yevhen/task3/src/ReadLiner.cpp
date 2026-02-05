#include "ReadLiner.h"
#include <algorithm>
#include <fstream>
#include "Printer.h"
#include <future>

Lines& Lines::operator+=(const Lines& other) {
        blank += other.blank;
        comment += other.comment;
        code += other.code;
        time_ms += other.time_ms;
        return *this;
    }

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

DirResult ReadLiner::processDirectory(const std::filesystem::path& dirPath, unsigned char currentDepth)
{
    DirResult dirResult;
    dirResult.lines = Lines{0,0,0,std::chrono::microseconds(0)};

    if (currentDepth > maxDepth_)
        return dirResult;

    dirResult.results.emplace_back(dirPath,
                                    Lines{0,0,0,std::chrono::microseconds(0)},
                                    0,
                                    currentDepth);

    int dirIndex = 0;
    std::vector<std::future<DirResult>> subdirFutures;
    std::vector<std::future<Lines>> fileFutures;
    std::vector<std::filesystem::path> filePaths;

    for (const auto& entry : std::filesystem::directory_iterator(dirPath))
    {
        if (entry.is_directory())
        {
            subdirFutures.push_back(
                std::async(std::launch::async,
                    [this, entry, currentDepth]() {
                        return this->processDirectory(entry.path(), currentDepth + 1);
                    }
                )
            );
        }
        else if (entry.path().extension() == ".cpp" ||
                 entry.path().extension() == ".h" ||
                 entry.path().extension() == ".hpp" ||
                 entry.path().extension() == ".c")
        {
            filePaths.push_back(entry.path());
            fileFutures.push_back(
                std::async(std::launch::async,
                    [this, entry]() {
                        return this->processFile(entry.path());
                    }
                )
            );
        }
    }

    for (auto& f : subdirFutures)
    {
        DirResult child = f.get();
        for (auto& r : child.results) {
            dirResult.results.push_back(r);
        }

        dirResult.lines += child.lines;
        if (!child.results.empty())
            std::get<2>(dirResult.results[dirIndex]) += std::get<2>(child.results[0]);
    }

    for (long long unsigned int i = 0; i < fileFutures.size(); i++)
    {
        Lines flines = fileFutures[i].get();
        const auto& path = filePaths[i];
        dirResult.results.emplace_back(path, flines, 1, currentDepth + 1);
        dirResult.lines += flines;

        std::get<2>(dirResult.results[dirIndex]) += 1;
    }

    dirResult.results[dirIndex] = std::make_tuple(dirPath, dirResult.lines, std::get<2>(dirResult.results[dirIndex]), currentDepth);

    return dirResult;
}


void ReadLiner::run(const std::filesystem::path& pathToSaveFile, SaveMode mode) {
    if (!validateDirectory(root_)) {
        throw std::runtime_error("Invalid root directory: " + root_.generic_string());
    }
    DirResult rootResult = processDirectory(root_, 0);
    results_ = std::move(rootResult.results);

    IPrinter* printer = new CSVPrinter();
    switch (mode)
    {
    case SaveMode::CSV:
        break;
    case SaveMode::Pretty:
        printer = new PrettyPrinter();
        break;
    default:
        break;
    }
    printer->save(pathToSaveFile, results_);
}