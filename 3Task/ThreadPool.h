#pragma once
#include <vector>
#include <filesystem>
#include "Counters.h"

class ThreadPool {
public:
    static Counters run(const std::vector<std::filesystem::path>& files);
};

