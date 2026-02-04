#pragma once
#include <filesystem>
#include "Counters.h"

namespace fs = std::filesystem;

class FileProcessor {
public:
    static void process_file(const fs::path& path, Counters& local);
};

