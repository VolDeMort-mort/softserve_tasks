#pragma once
#include <string>
#include "Statistics.h"

class ReportWriter {
public:
    static void write(const std::string& file_path, const TotalStats& stats);
};
