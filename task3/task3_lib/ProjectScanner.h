#pragma once
#include "pch.h"
#include "FileResult.h"
#include "ProjectResult.h"
#include "AbstractResultWriter.h"

class ProjectScanner
{
private:
	std::vector<std::filesystem::path> m_projectFiles;
	std::filesystem::path m_strDirectory;
	size_t m_nThreads;
	std::unique_ptr<AbstractResultWriter> m_writer;

	void getFilesFromDirectory();
public:
	ProjectScanner(const std::filesystem::path& strDirectory, std::unique_ptr<AbstractResultWriter> writer);

	static FileResult analyzeFile(const std::filesystem::path& path);

	void proccess();
	void changeDirectory(const std::filesystem::path& strDirectory);
};

