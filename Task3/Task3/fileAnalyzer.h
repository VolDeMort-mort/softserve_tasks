#pragma once

#include <filesystem>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <atomic>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#define DEBUG_SLEEP_MODE 0
#define DEBUG_PRINT_MODE 0

#if (DEBUG_PRINT_MODE != 0)
inline std::mutex DEBUG_MUTEX;

#define DEBUG_PRINT(X) do { \
        std::scoped_lock<std::mutex> lock(DEBUG_MUTEX); \
        std::cout << X; \
    } while(0)
#else
#define DEBUG_PRINT(X)
#endif

struct fileStats {
	std::atomic<size_t> lines{0};
	std::atomic<size_t> comments{0};
	std::atomic<size_t> blank{0};
	std::atomic<size_t> code{ 0 };
	std::atomic<size_t> filesAnalyzed{0};
	std::atomic<size_t> filesFound{0};

	std::string defaultPath = "";

	std::chrono::high_resolution_clock::time_point timerBegin;
	std::chrono::high_resolution_clock::time_point timerEnd;

	const std::vector<std::string> extensions = {
		".cpp",
		".c",
		".h",
		".hpp",
	};


	fileStats(const fileStats& other) {
		lines.store(other.lines.load());
		comments.store(other.comments.load());
		blank.store(other.blank.load());
		code.store(other.code.load());
		filesAnalyzed.store(other.filesAnalyzed.load());
		filesFound.store(other.filesFound.load());
		defaultPath = other.defaultPath;
		timerBegin = other.timerBegin;
		timerEnd = other.timerEnd;
	}

	fileStats& operator=(const fileStats& other) {
		if (this != &other) {
			lines.store(other.lines.load());
			comments.store(other.comments.load());
			blank.store(other.blank.load());
			code.store(other.code.load());
			filesAnalyzed.store(other.filesAnalyzed.load());
			filesFound.store(other.filesFound.load());
			defaultPath = other.defaultPath;
			timerBegin = other.timerBegin;
			timerEnd = other.timerEnd;
		}
		return *this;
	}

	fileStats() = default;
};


class fileAnalyzer {
public:
	fileAnalyzer() = default;

	size_t init(const std::filesystem::path& dirPath, bool nested = false, std::function<void(size_t all)> begin = nullptr,
	            std::function<void(const std::string& filename)> process = nullptr,
	            std::function<void(fileStats total, bool status)> end = nullptr);

	void run();

	fileStats getStats() {
		std::scoped_lock<std::mutex> lock(statsMutex);
		return stats;
	}

	size_t getQueueLen() const { return filesQueue.size(); }

	bool saveToFile(std::string path, std::string name) const;

private:
	std::filesystem::path defaultPath;
	bool isNested;

	fileStats stats;
	std::mutex statsMutex;

	std::function<void(size_t all)> onBegin = nullptr;
	std::function<void(const std::string& filename)> onFileAnalyzeBegin = nullptr;
	std::function<void(fileStats total, bool status)> onFinish = nullptr;

	std::queue<std::filesystem::path> filesQueue;


	bool checkFileExtension(const std::filesystem::path& p) const noexcept;

	void processFile(std::filesystem::path path);
};
