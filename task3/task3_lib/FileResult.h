#pragma once
#include "pch.h"

struct FileResult
{
    std::filesystem::path path;
    uint64_t blanks = 0;
    uint64_t comments = 0;
    uint64_t code = 0;
    bool opened = false;
};

