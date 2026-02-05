#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <ThreadPool.h>


enum class mode
{
	etype_single,
	etype_multi,
	etype_optinal
};


struct statistics
{
	std::atomic<int> total_files{ 0 };
	std::atomic<int> blank_lines{ 0 };
	std::atomic<int> comment_lines{ 0 };
	std::atomic<int> code_lines{ 0 };
	double time{ 0.0 };
};


class Reader
{
public:

	Reader(const std::vector<std::string>& file_path) : filePathVec(file_path)
	{
		data = std::make_shared<statistics>();
	}

	void read_file(mode mode);
	std::shared_ptr<statistics> getData() const;

private:
	void read_single();
	void read_multi();
	std::string reform_line(std::string line);
	void parse_data(std::string& line, bool& multi_comm);
	void process_multi_comment(std::string& line, size_t& position, bool& multi_comm);
	bool find_next_comment(const std::string& line, size_t position, size_t& next_comment_pos, bool& is_single);
	void process_comment_and_code(const std::string& line, size_t& position, size_t next_comment_pos, bool is_single, bool& multi_comm);

private:
	std::string filePath;
	const std::vector<std::string>& filePathVec;
	std::shared_ptr<statistics> data;
};
