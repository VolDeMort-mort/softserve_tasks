#include "pch.h"
#include "ProjectScanner.h"

ProjectScanner::ProjectScanner(const std::filesystem::path& strDirectory, std::unique_ptr<AbstractResultWriter> writer)
	: m_strDirectory(strDirectory), m_writer(std::move(writer)), m_nThreads(std::thread::hardware_concurrency())
{
	if (m_nThreads == 0) m_nThreads = 4;
}

void ProjectScanner::changeDirectory(const std::filesystem::path& strDirectory) {
	m_strDirectory = strDirectory;
}

void ProjectScanner::getFilesFromDirectory() {
	m_projectFiles.clear();
	try {
		for (auto const& file : std::filesystem::recursive_directory_iterator(m_strDirectory)) {
			if (file.is_regular_file()) {
				std::string ext = file.path().extension().string();
				for (char& c : ext) c = std::tolower(static_cast<unsigned char>(c));
				if (ext == ".cpp" || ext == ".c" || ext == ".hpp" || ext == ".h") {
					m_projectFiles.push_back(file.path());
				}
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

/*
Logic of line separation
if line is blank - counts as blank
if there is code in the line - counts as code
if thers are only comments - counts as comment
For example
fun.do(); // function doing something - is a code line
// comment - is a comment line
/*comment*./ i++; - is a code line
*/

FileResult ProjectScanner::analyzeFile(const std::filesystem::path& path) {
	FileResult result;
	result.path = path;
	std::ifstream fin(path);
	if (!fin.is_open()) {
		result.opened = false;
		return result;
	}
	result.opened = true;
	std::string line;
	bool inCommentBlock = false;

	while (std::getline(fin, line)) {
		//only space line check
		bool onlySpaces = true;
		for (unsigned char c : line) {
			if (!std::isspace(c)) {
				onlySpaces = false;
				break;
			}
		}
		if (onlySpaces) {
			result.blanks++;
			continue;
		}

		size_t i = 0;
		size_t size = line.size();
		bool seenCode = false;
		while (i < size) {
			if (!inCommentBlock) {
				if (i + 1 < size && line[i] == '/' && line[i + 1] == '/') {
					break; // no need to cheak further
				}
				if (i + 1 < size && line[i] == '/' && line[i + 1] == '*') {
					inCommentBlock = true;
					i += 2;
					continue;
				}
				// if its not in the comment block and starting not with '/'
				// and not a space, its a code
				if (!std::isspace(static_cast<unsigned char>(line[i]))) { 
					seenCode = true;
				}
				++i;
			}
			else {
				if (i + 1 < size && line[i] == '*' && line[i + 1] == '/') {
					inCommentBlock = false;
					i += 2;
				}
				else {
					++i;
				}
			}
		}
		if (seenCode) result.code++;
		else result.comments++;
	}

	fin.clear();
	fin.seekg(0, std::ios::end);
	std::streamoff pos = fin.tellg();
	if (pos > 0) {
		fin.seekg(-1, std::ios::cur);
		char lastChar = 0;
		fin.get(lastChar);
		if (lastChar == '\n') {
			result.blanks++;
		}
	}

	fin.close();
	return result;
}

void ProjectScanner::proccess() {
	using clk = std::chrono::steady_clock;
	auto t0 = clk::now();

	getFilesFromDirectory();

	const size_t nFiles = m_projectFiles.size();
	std::vector<FileResult> results;
	results.resize(nFiles);

	std::atomic<size_t> nextIndex(0); // ensure that there is no race condition

	size_t threadsToCreate = std::min(m_nThreads, nFiles);
	std::vector<std::thread> threads;
	threads.reserve(threadsToCreate);

	for (int i = 0; i < threadsToCreate; ++i) {
		threads.emplace_back([this, &nextIndex, &results, nFiles]() {
			while (true) {
				size_t idx = nextIndex.fetch_add(1, std::memory_order_relaxed);
				if (idx >= nFiles) break;
				results[idx] = analyzeFile(m_projectFiles[idx]);
			}
		});
	}

	for (std::thread& thr : threads) {
		thr.join();
	}

	auto t1 = clk::now();
	std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

	ProjectResult prResult(std::move(results), time, m_strDirectory);

	if (m_writer) {
		m_writer->write(prResult);
	}
	else {
		std::cerr << "No writer provided\n";
	}

}