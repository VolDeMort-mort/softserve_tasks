#pragma once
#include <string>
#include <filesystem>
class Searcher
{
public:
	void startSearch(std::string path);
	bool checkExtension(std::filesystem::path& extension);
};

