#pragma once
#include <filesystem>
#include "Collector.h"
class Counter
{
public:
	LinesResult readFile(const std::filesystem::directory_entry& file);
};

