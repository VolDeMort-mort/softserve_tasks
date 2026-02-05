#pragma once

#include <filesystem>  
#include <string>
#include <cstdint>     

namespace fs = std::filesystem;

class FileScanner {
private:
    fs::path rootPath_;      
    std::vector<fs::path> files_;

public:
    explicit FileScanner(const std::string& rootPath);

    void scan();
    const std::vector<fs::path>& getFiles() const { return files_; }

};
