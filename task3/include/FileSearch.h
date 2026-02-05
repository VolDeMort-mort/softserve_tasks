#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <Timer.h>

class FileSearch
{
public:
	FileSearch(std::string path)
	{
		root = path;
		Timer::timer_start();
	}

	std::vector<std::string> filesPath_search();

private:
	std::string root;
};
