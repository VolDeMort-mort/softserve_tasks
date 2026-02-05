#include <iostream>
#include <filesystem>
#include <queue>
#include "ThreadPool.h"
#include "ReportWriter.h"

int main()
{
	ThreadPool thread_pool;
	thread_pool.launchThreads();
	std::filesystem::path root = "C:/Users/Legion/source/repos/SoftServe/Task1";
	for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
		if (entry.is_regular_file()) {
			std::filesystem::path extension = entry.path().extension();
			if (extension == ".h" || extension == ".hpp" || extension == ".c" || extension == ".cpp") {
				thread_pool.addFilePath(entry.path());
			}
		}
	}
	thread_pool.finishAdding();
	thread_pool.joinThreads();
	ReportWriter::writeToFile(thread_pool.getResults(), "results.txt");
}