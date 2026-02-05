#pragma once
#include <filesystem>

struct FileStat{
	std::filesystem::path path;
	std::string name;
	uint32_t blank_lines_count = 0;
	uint32_t comment_lines_count = 0;
	uint32_t code_lines_count = 0;
	double execution_time = 0;
};