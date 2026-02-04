#pragma once
#include <filesystem>

class Application {
public:
    int run(const std::filesystem::path& root);
};

