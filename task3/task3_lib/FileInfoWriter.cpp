#include "pch.h"
#include "FileInfoWriter.h"

std::string FileInfoWriter::formatRelativePath(const std::filesystem::path& fullPath,
	const std::filesystem::path& baseDir,
	int maxWidth)
{
	std::filesystem::path rel;

	try {
		rel = std::filesystem::relative(fullPath, baseDir);
	}
	catch (...) {
		rel = fullPath;
	}

	std::string strPath = rel.string();

	if (strPath.size() <= maxWidth)
		return strPath;

	return "..." + strPath.substr(strPath.size() - (maxWidth - 3));
}


void FileInfoWriter::write(const ProjectResult& prResult) {
	std::ofstream fout(m_file, std::ios::out | (m_isAppend ? std::ios::app : std::ios::trunc));
	if (!fout) {
		std::cerr << "Failed to open output file: " << m_file << '\n';
		return;
	}

	uint64_t totalBlanks = 0;
	uint64_t totalCode = 0;
	uint64_t totalComments = 0;
	uint64_t openedCount = 0;


	size_t size = prResult.m_filesResults.size();
	const std::vector<FileResult>& filesResults(prResult.m_filesResults);

	for (int i = 0; i < size; ++i) {
		if (filesResults[i].opened) {
			totalBlanks += filesResults[i].blanks;
			totalComments += filesResults[i].comments;
			totalCode += filesResults[i].code;
			++openedCount;
		}
	}

	uint64_t failedToOpen = size - openedCount;
	uint64_t totalLines = totalBlanks + totalComments + totalCode;

	int widthStatusName = 30;
	int widthParam = 12;
	int widthFileName = 50;
	int widthTable = widthFileName + 5 * widthParam;

	fout << std::string(widthTable, '-') << '\n';
	fout << "Project report for directory: " << prResult.m_dir << "\n";
	fout << "Scan time (ms): " << prResult.m_time_ms.count() << "\n\n";

	fout << std::left << std::setw(widthStatusName) << "Files scanned:" << size << "\n";
	fout << std::left << std::setw(widthStatusName) << "Files opened:" << openedCount << "\n";
	fout << std::left << std::setw(widthStatusName) << "Files failed to open:" << failedToOpen << "\n\n";

	fout << std::left << std::setw(widthStatusName) << "Total lines (all files):" << totalLines << "\n";
	fout << std::left << std::setw(widthStatusName) << "Total code lines:" << totalCode << "\n";
	fout << std::left << std::setw(widthStatusName) << "Total comment lines:" << totalComments << "\n";
	fout << std::left << std::setw(widthStatusName) << "Total blank lines:" << totalBlanks << "\n\n";
	
	if (size > 0) {
		fout << "File details:\n";
		fout << std::left
			<< std::setw(widthFileName) << "File"
			<< std::setw(widthParam) << "Opened"
			<< std::setw(widthParam) << "Code"
			<< std::setw(widthParam) << "Comments"
			<< std::setw(widthParam) << "Blanks"
			<< std::setw(widthParam) << "Total"
			<< "\n";

		for (size_t i = 0; i < size; ++i) {
			const FileResult& fr = filesResults[i];
			uint64_t fileTotal = fr.code + fr.comments + fr.blanks;
			std::string name = formatRelativePath(fr.path, prResult.m_dir, widthFileName);
			fout << std::left
				<< std::setw(widthFileName) << name
				<< std::setw(widthParam) << (fr.opened ? "yes" : "no")
				<< std::setw(widthParam) << fr.code
				<< std::setw(widthParam) << fr.comments
				<< std::setw(widthParam) << fr.blanks
				<< std::setw(widthParam) << fileTotal
				<< "\n";
		}
	}
	fout << std::string(widthTable, '-') << '\n';

	fout.close();
}