#include "Searcher.h"
#include "Counter.h"
#include "Collector.h"
#include "ThreadPool.h"
#include "Reporter.h"
#include <filesystem>

namespace fs = std::filesystem;

void Searcher::startSearch(std::string path)
{
	std::error_code ec;
	auto it = fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied, ec);
	if (ec)
		throw std::runtime_error(ec.message());

	Counter counter;
	Collector collector;
	ThreadPool thrPool;

	auto start = std::chrono::high_resolution_clock::now();
	for (const auto& file : it){
		auto extension = file.path().extension();
		
		if(checkExtension(extension)) // if extension correct push file to queue
			thrPool.setToQueue(file);
	}
	thrPool.startThreads(counter,collector);
	
	auto end = std::chrono::high_resolution_clock::now();
	auto timeMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	collector.setTime(timeMS);

	LinesResult fileStat = collector.getResult();

	Reporter reporter("D:\\Programming\\source\\repos\\Task 3\\Task 3\\result.txt");
	reporter.setReport(fileStat);
}
bool Searcher::checkExtension(std::filesystem::path& extension)
{
	if (extension == ".c" || extension == ".cpp" 
		|| extension == ".h" || extension == ".hpp")
		return true;
	return false;
}
