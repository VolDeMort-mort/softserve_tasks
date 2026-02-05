#pragma once
#include <queue>
#include <filesystem>
#include <thread>
#include <condition_variable>
#include "FileStat.h"

class ThreadPool{
private:
	std::queue<std::filesystem::path> queue;
	std::vector<std::thread> workers;
	std::condition_variable cv;
	std::mutex mutex;

	std::vector<FileStat> results;
	std::mutex results_mutex;

	bool finished_adding = false;

	int NUM_OF_THREADS = 4;
public:
	void addFilePath(const std::filesystem::path& file);
	void workerLoop();
	void finishAdding();
	void launchThreads();
	void joinThreads();

	const std::vector<FileStat>& getResults() const{
		return results;
	}
};

