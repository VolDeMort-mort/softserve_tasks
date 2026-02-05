#include "fileAnalyzer.h"

#include <random>

size_t fileAnalyzer::init(const std::filesystem::path& dirPath, bool nested, std::function<void(size_t all)> begin,
                          std::function<void(const std::string& filename)> process,
                          std::function<void(fileStats total, bool status)> end) {
	defaultPath = dirPath;
	isNested = nested;
	onBegin = std::move(begin);
	onFileAnalyzeBegin = std::move(process);
	onFinish = std::move(end);

	if (isNested) {
		for (const auto& p :
		     std::filesystem::recursive_directory_iterator(defaultPath,
		                                                   std::filesystem::directory_options::skip_permission_denied)) {
			if (p.is_regular_file() && checkFileExtension(p.path())) { filesQueue.push(p.path()); }
		}
	}
	else {
		for (const auto& p : std::filesystem::directory_iterator(defaultPath)) {
			if (p.is_regular_file() && checkFileExtension(p.path())) filesQueue.push(p.path());
		}
	}


	stats.filesFound.store(filesQueue.size());
	stats.defaultPath = defaultPath.string();
	return stats.filesFound;
}

void fileAnalyzer::run() {

	stats.timerBegin = std::chrono::high_resolution_clock::now();

	size_t availableThreads = std::thread::hardware_concurrency();
	if (!availableThreads) availableThreads = 2;

	std::vector<std::thread> threadsPool;
	std::mutex queueMutex;

	DEBUG_PRINT("Starting analysis with " << availableThreads << " threads...\n");

	threadsPool.reserve(availableThreads);

	if (onBegin) { onBegin(stats.filesFound); }

	for (size_t i = 0; i < availableThreads; ++i) {
		threadsPool.emplace_back([this, &queueMutex] {
			while (true) {

#if (DEBUG_SLEEP_MODE != 0)
				static thread_local std::mt19937 rng{ std::random_device{}() };
				std::uniform_int_distribution<int> dist(0, DEBUG_SLEEP_MODE);

				std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
				DEBUG_PRINT("Thread " << std::this_thread::get_id() << " woke up\n");
#endif

				std::filesystem::path file;
				{
					std::scoped_lock<std::mutex> lock(queueMutex);
					if (filesQueue.empty()) return;

					file = std::move(filesQueue.front());
					filesQueue.pop();

					if (onFileAnalyzeBegin) onFileAnalyzeBegin(file.string());
				}

				processFile(file);
			}
		});
	}

	for (auto& t : threadsPool) { if (t.joinable()) t.join(); }

	stats.timerEnd = std::chrono::high_resolution_clock::now();

	if (onFinish) onFinish(stats, true);

}

bool fileAnalyzer::saveToFile(std::string path, std::string name) const
{
	namespace fs = std::filesystem;

	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm local_tm;
	localtime_s(&local_tm, &now_time);

	std::stringstream ss;
	ss << std::put_time(&local_tm, "%d.%m.%Y, %H:%M:%S");
	std::string dateStr = ss.str();

	if (name.empty()) {
		std::stringstream ss;
		ss << "SHARK_REPORT_" << std::put_time(&local_tm, "%d-%m_%H-%M") << ".txt";
		name = ss.str();
	}
	else if (!name.ends_with(".txt")) {
		name.append(".txt");
	}

	fs::path fullPath = fs::path(path) / name;

	try {
		fs::path dir = fullPath.parent_path();
		if (!dir.empty() && !fs::exists(dir)) {
			if (!fs::create_directories(dir)) {
				return false;
			}
		}

		std::ofstream file(fullPath, std::ios::out | std::ios::trunc);

		if (!file.is_open()) {
			return false;
		}

		file << "===== >>>>> FILE SHARK REPORT <<<<< ======\n";
		file << "------------------------------------------\n";
		file << "Files Found: " << stats.filesFound.load() << "\n";
		file << "Files Analyzed: " << stats.filesAnalyzed.load() << "\n";
		file << "Analysis Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stats.timerEnd - stats.timerBegin).count() << " ms\n";
		file << "Total Lines: " << stats.lines.load() << "\n";
		file << "Code Lines: " << stats.code.load() << "\n";
		file << "Comments: " << stats.comments.load() << "\n";
		file << "Blank Lines: " << stats.blank.load() << "\n";
		file << "------------------------------------------\n";
		file << "Report generated: " << dateStr << "\n";

		file.close();
		return true;
	}
	catch (const fs::filesystem_error& e) {
		return false;
	}
}

bool fileAnalyzer::checkFileExtension(const std::filesystem::path& p) const noexcept {
	std::string extension = p.extension().string();
	for (const auto& el : stats.extensions) { if (extension == el) { return true; } }
	return false;
}

void fileAnalyzer::processFile(std::filesystem::path path) {
	std::ifstream file(path);
	if (!file.is_open()) { return; }

	size_t physicalLines = 0;
	size_t commentLines = 0;
	size_t blankLines = 0;
	size_t codeLines = 0;

	bool inCommentBlock = false;

	std::string currentLine;

	while (std::getline(file, currentLine)) {
		physicalLines++;

		size_t firstLetter = currentLine.find_first_not_of(" \t\n\r");

		if (firstLetter == std::string::npos) {
			if (inCommentBlock) { commentLines++; }
			else { blankLines++; }
			continue;
		}

		if (inCommentBlock) {
			commentLines++;

			if (currentLine.find("*/") != std::string::npos) { inCommentBlock = false; }
		}
		else {
			bool isComment = false;

			if (firstLetter + 1 >= currentLine.length()) { continue; }

			if (currentLine.compare(firstLetter, 2, "//") == 0) {
				commentLines++;
				isComment = true;
			}
			else if (currentLine.compare(firstLetter, 2, "/*") == 0) {
				commentLines++;
				isComment = true;
				if (currentLine.find("*/") == std::string::npos) inCommentBlock = true;
			}
			if (!isComment) { codeLines++; }
		}
	}

	stats.lines.fetch_add(physicalLines);
	stats.comments.fetch_add(commentLines);
	stats.blank.fetch_add(blankLines);
	stats.code.fetch_add(codeLines);
	stats.filesAnalyzed.fetch_add(1);
}
