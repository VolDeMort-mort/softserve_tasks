#include "Counter.h"
#include <filesystem>
#include <fstream>
#include <string>
#include "Collector.h"
#include <iostream>


LinesResult Counter::readFile(const std::filesystem::directory_entry& file)
{
	LinesResult result;
	std::ifstream fileRead(file.path());

	if (!fileRead.is_open())
	{
		std::cerr << "Could not open file " << file.path().string() << "\n";
		++result.error_files;
		return result;
	}

	std::string str;
	bool in_comment_block = false;
	
	while (std::getline(fileRead, str))
	{
		bool codeStatus = false;
		bool commentStatus = false;
		bool in_string = false;
		if (str.empty() && !in_comment_block)
		{
			++result.blank_lines;
			continue;
		}
		else if(str.empty() && in_comment_block)
		{
			++result.comment_lines;
			continue;
		}
		for (size_t i = 0; i < str.size(); ++i) {
			bool isNext = (i + 1 < str.size());
			if (str[i] == '"' && !in_comment_block)
			{
				in_string = !in_string;
				codeStatus = true;
			}

			if (in_comment_block) {
				commentStatus = true;
				if (isNext && str[i] == '*' && str[i + 1] == '/')
				{
					in_comment_block = false;
					i++; // to skip '/' symbol
				}
			}
			else {

				//cast to Uchar to handle char from 128 to 255 in ASCII
				if (isspace(static_cast<unsigned char>(str[i]))) continue; 

				if (isNext && str[i] == '/' && str[i + 1] == '/' && !in_string)
				{
					commentStatus = true;
					break;
				}
				else if (isNext && str[i] == '/' && str[i + 1] == '*' && !in_string)
				{
					in_comment_block = true;
					commentStatus = true;
					i++; // to skip '*' symbol
				}
				else
					codeStatus = true;
			}
		}
		if (codeStatus) ++result.code_lines;
		else if (commentStatus) ++result.comment_lines;
		

	}
	return result;
}
