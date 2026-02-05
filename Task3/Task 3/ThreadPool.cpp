#include "ThreadPool.h"
#include "Counter.h"
#include "Searcher.h"
#include "Collector.h"

ThreadPool::ThreadPool() : threadCount(std::thread::hardware_concurrency())
{
	if (threadCount == 0) 
		threadCount = 2; // default minimum and safe for single core or any system
}
int ThreadPool::getThreadCount()
{
	return threadCount;
}
void ThreadPool::setToQueue(const std::filesystem::directory_entry& file)
{
	workQ.push(file.path().string());
}
void ThreadPool::startThreads(Counter& counter,Collector& collector)
{
	if (workQ.empty())
		throw std::runtime_error("Queue is empty");
	std::vector<std::thread> threads;

	for (size_t i = 0; i < threadCount; ++i) {
		threads.emplace_back(std::thread([this, &collector]() {
			Counter lambdaCounter;
			while (true)
			{
				std::string fileStr;
				{
					std::lock_guard<std::mutex> lock(this->thrMtx);
					if (workQ.empty())
						break;

					fileStr = std::move(workQ.front());
					workQ.pop();
				}
				std::filesystem::directory_entry file(fileStr);
				LinesResult fileStats = lambdaCounter.readFile(file);
				collector.addFileStats(fileStats);
			}
		}));
	}
	for (auto& t : threads) {
	if(	t.joinable())
		t.join();
	}
}
