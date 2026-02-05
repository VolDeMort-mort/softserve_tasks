#include "Reader.h"
#include "Timer.h"


void Reader::read_single()
{
	for (const auto& f : filePathVec)
	{
		bool multi_comm = false;
		std::ifstream file_to_read;

		file_to_read.open(f);
		if (file_to_read.is_open())
		{
			data->total_files++;

			std::string line;
			while (std::getline(file_to_read, line))
			{
				parse_data(line, multi_comm);

			}
		}

		file_to_read.close();
	}
}


void Reader::read_multi()
{
	ThreadPool pool;
	for (const auto& f : filePathVec)
	{
		pool.add_task([f, this]()
			{
				bool multi_comm = false;

				std::ifstream file_to_read(f);
				if (file_to_read.is_open())
				{
					std::string line;
					while (std::getline(file_to_read, line))
					{
						parse_data(line, multi_comm);

					}
				}
			});
	}

	pool.wait_for_tasks();
}


void Reader::read_file(mode mode)
{
	switch (mode)
	{

	case mode::etype_single:
		read_single();
		break;

	case mode::etype_multi:
		read_multi();
		break;

	case mode::etype_optinal:
		if (filePathVec.size() < 300)
		{
			read_file(mode::etype_single);
		}
		else
		{
			read_file(mode::etype_multi);
		}
		break;

	default:
		break;
	}

	data->total_files = filePathVec.size();

	Timer::timer_end();
	data->time = Timer::get_time();
		
}


std::string Reader::reform_line(std::string line)
{
	int line_start = 0;
	int end_line = line.size() - 1;

	if (line.empty())
		return "";

	while (line_start < line.size() && (line[line_start] == ' ' || line[line_start] == '\t'))
	{
		line_start++;
	}

	while (end_line >= 0 && (line[end_line] == ' ' || line[end_line] == '\t'))
	{
		end_line--;
	}

	if (end_line >= line_start)
	{
		line = line.substr(line_start, end_line - line_start + 1);
	}
	else
	{
		return "";
	}

	return line;
}


void Reader::process_multi_comment(std::string& line, size_t& position, bool& multi_comm) 
{
	if (multi_comm) 
	{
		data->comment_lines++;
		size_t multi_comm_pos_end = line.find("*/");

		if (multi_comm_pos_end != std::string::npos)
		{
			multi_comm = false;
			position = multi_comm_pos_end + 2;
		}
		else
		{
			position = line.size();
		}
	}
}


bool Reader::find_next_comment(const std::string& line, size_t position, size_t& next_comment_pos, bool& is_single) 
{
	size_t multi_comm_pos_start = line.find("/*", position);
	size_t str_comm_pos = line.find("//", position);

	is_single = false;
	next_comment_pos = std::string::npos;

	if (str_comm_pos != std::string::npos && (multi_comm_pos_start == std::string::npos || str_comm_pos < multi_comm_pos_start)) 
	{
		next_comment_pos = str_comm_pos;
		is_single = true;
		return true;
	}
	else if (multi_comm_pos_start != std::string::npos)
	{
		next_comment_pos = multi_comm_pos_start;
		return true;
	}

	return false;
}

void Reader::process_comment_and_code(const std::string& line, size_t& position, size_t next_comment_pos, bool is_single, bool& multi_comm) 
{

	if (!reform_line(line.substr(position, next_comment_pos - position)).empty())
		data->code_lines++;

	if (is_single) 
	{
		data->comment_lines++;
		position = line.size(); 
		return;
	}

	size_t multi_comm_pos_end = line.find("*/", next_comment_pos + 2);
	if (multi_comm_pos_end == std::string::npos) 
	{
		data->comment_lines++;
		multi_comm = true;
		position = line.size();
		return;
	}

	data->comment_lines++;
	position = multi_comm_pos_end + 2;
}

void Reader::parse_data(std::string& line, bool& multi_comm)
{
	line = reform_line(line);
	size_t position = 0;

	if (line.empty())
	{
		data->blank_lines++;
		return;
	}

	process_multi_comment(line, position, multi_comm);

	while (position < line.size())
	{
		size_t next_comment_pos;
		bool is_single;

		if (!find_next_comment(line, position, next_comment_pos, is_single)) 
		{
			if (!reform_line(line.substr(position)).empty())
				data->code_lines++;

			break;
		}

		process_comment_and_code(line, position, next_comment_pos, is_single, multi_comm);
	}

}

std::shared_ptr<statistics> Reader::getData() const
{
	return data;
}
