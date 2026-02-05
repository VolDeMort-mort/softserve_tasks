#pragma once
#include "pch.h"
#include "FileResult.h"

struct ProjectResult{
	ProjectResult() = default;
	ProjectResult(std::vector<FileResult> filesResults, std::chrono::milliseconds time_ms, const std::filesystem::path& dir)
		: m_filesResults(std::move(filesResults)), m_time_ms(time_ms), m_dir(dir)
	{}

	std::vector<FileResult> m_filesResults;
	std::chrono::milliseconds m_time_ms;
	std::filesystem::path m_dir;
};