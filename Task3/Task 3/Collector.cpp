#include "Collector.h"		

void Collector::addFileStats(const LinesResult& fileStats)
{
	std::lock_guard<std::mutex> lock(mtx);
	stats.blank_lines += fileStats.blank_lines;
	stats.comment_lines += fileStats.comment_lines;
	stats.code_lines += fileStats.code_lines;
	stats.error_files += fileStats.error_files;
	++stats.processed_files;
}
LinesResult Collector::getResult() const
{
	return stats;
}
void Collector::setTime(long long time)
{
	stats.timeMS = time;
}