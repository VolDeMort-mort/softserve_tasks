#pragma once
#include <cstdint>


struct FileStats {
	uint32_t blank_lines{};
	uint32_t comment_lines{};
	uint32_t code_lines{};
};


struct TotalStats {
	uint32_t blank_lines{};
	uint32_t comment_lines{};
	uint32_t code_lines{};
	uint32_t file_count{};
	double execution_time{};

	void add_file(const FileStats& f);
};

