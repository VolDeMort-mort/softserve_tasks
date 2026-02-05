#include "FileScanner.h"
#include <iostream>

FileScanner::FileScanner(const std::string& rootPath)
    : rootPath_(rootPath) {
    if (!fs::exists(rootPath_)) {
        throw std::runtime_error("Root does not exist: " + rootPath);
    }
}


static bool hasValidExtension(const fs::path& p) {
    auto ext = p.extension().string();
    return ext == ".h" || ext == ".hpp" || ext == ".c" || ext == ".cpp";
}

void FileScanner::scan() {
    files_.clear();
    try {
        for (const auto& entry : fs::recursive_directory_iterator(rootPath_)) {
            if (entry.is_regular_file() && hasValidExtension(entry.path())) {
                files_.push_back(entry.path());
            }
        }
    }
    catch (const fs::filesystem_error& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }
}
