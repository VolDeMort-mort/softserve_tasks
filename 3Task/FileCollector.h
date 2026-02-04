#pragma once
#include <vector>
#include <filesystem>

class FileCollector {
public:
    static std::vector<std::filesystem::path>
    collect(const std::filesystem::path& root);
};
