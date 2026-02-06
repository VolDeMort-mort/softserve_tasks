#include "FileUtils.h"

#include <vector>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cctype>

std::vector<std::string> findSourceFiles(const std::string& root) {
	std::vector<std::string> files;
	const std::vector<std::string> extensions = { ".c", ".cpp", ".h", ".hpp" };

	try {
		for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
			if (!entry.is_regular_file()) continue;

			std::string ext = entry.path().extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(),
				[](unsigned char c) { return std::tolower(c); });

			if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
				files.push_back(entry.path().string());
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cout << "Error: Cant find source file: " << e.what() << std::endl;
	}
	return files;
}