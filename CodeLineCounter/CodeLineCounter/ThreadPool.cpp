#include "ThreadPool.h"
#include "FileProcessor.h"

void ThreadPool::addFilePath(const std::filesystem::path& file){
	queue.push(file);
}

void ThreadPool::workerLoop(){
	while (true) {
		std::filesystem::path file_path;
		{
			std::unique_lock<std::mutex> lock(mutex);
			if (!finished_adding) {
				cv.wait(lock);
			}

			if (queue.empty() && finished_adding) {
				return;
			}

			file_path = queue.front();
			queue.pop();
		}

		FileStat file_stat = FileProcessor::processFile(file_path);
		std::unique_lock<std::mutex> results_lock(results_mutex);
		results.push_back(file_stat);
	}
}

void ThreadPool::finishAdding(){
	finished_adding = true;
	cv.notify_all();
}

void ThreadPool::launchThreads(){
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		workers.emplace_back(&ThreadPool::workerLoop, this);
	}
}

void ThreadPool::joinThreads(){
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		workers[i].join();
	}
}
