#pragma once
#include <mutex>

struct LinesResult {
	long long processed_files = 0;
	long long blank_lines = 0;
	long long comment_lines = 0;
	long long code_lines = 0;
	long long timeMS = 0;
	long long error_files = 0;
};

class Collector
{
private:
	std::mutex mtx;
	LinesResult stats;
public:
	void addFileStats(const LinesResult& fileStats);
	void setTime(long long time);
	LinesResult getResult() const;
};