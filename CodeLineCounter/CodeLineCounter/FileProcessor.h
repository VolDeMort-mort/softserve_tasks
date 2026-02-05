#pragma once
#include "FileStat.h"
class FileProcessor{
public:
	static FileStat processFile(const std::filesystem::path& path);
private:
	static void trim(std::string& s);
};

