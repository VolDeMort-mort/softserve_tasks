#pragma once
#include <queue>
#include <string>
#include <mutex>
#include <filesystem>
class Counter;
class Collector;
class ThreadPool
{
private:
	std::queue<std::string> workQ;
	int threadCount;
	std::mutex thrMtx;
public:
	ThreadPool();
	int getThreadCount();
	void setToQueue(const std::filesystem::directory_entry& file);
	void startThreads(Counter& counter,  Collector& collector);
};

