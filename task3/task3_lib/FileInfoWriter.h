#pragma once
#include "AbstractResultWriter.h"
class FileInfoWriter : public AbstractResultWriter
{
private:
	std::filesystem::path m_file;
	bool m_isAppend;

	std::string formatRelativePath(const std::filesystem::path& fullPath,
		const std::filesystem::path& baseDir,
		int maxWidth);
public:
	FileInfoWriter(const std::filesystem::path& file, bool isAppend = true) : m_file(file), m_isAppend(isAppend) {}

	virtual void write(const ProjectResult& prResult) override;
};

