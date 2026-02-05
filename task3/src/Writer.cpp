#include "Writer.h"
#include "Reader.h"

Writer::Writer(std::shared_ptr<statistics> data)
    : data_to_write(std::move(data)) {}

void Writer:: write_to_file()
{
	std::ofstream writer;
	writer.open("Statistics.txt");
	if (!writer.is_open())
	{
		throw std::runtime_error("Opening error");
	}
	writer << "Total files:   " << data_to_write->total_files.load() << '\n';
	writer << "Blank lines:   " << data_to_write->blank_lines.load() << '\n';
	writer << "Comment lines: " << data_to_write->comment_lines.load() << '\n';
	writer << "Code lines:    " << data_to_write->code_lines.load() << '\n';
	writer << "Time:          " << data_to_write->time << '\n';

}