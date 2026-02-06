#include "FileSearch.h"

std::vector<std::string>  FileSearch::filesPath_search()
{
	std::vector<std::string> filePathVec;

	for (const auto& file : std::filesystem::recursive_directory_iterator(root, std::filesystem::directory_options::skip_permission_denied))
	{
		if (file.is_regular_file())
		{
			std::string extension = file.path().extension().string();
			if (extension == ".cpp" || extension == ".h" || extension == ".hpp")
			{
				filePathVec.push_back(file.path().string());
			}
		}

	}

	return filePathVec;
}