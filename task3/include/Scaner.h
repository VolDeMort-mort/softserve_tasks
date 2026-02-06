#include "FileSearch.h"
#include "Reader.h"
#include "Writer.h"

class Scaner
{
public:
	Scaner(std::string path, mode mode)
	{
		root = path;
		m_ = mode;
	}

	void run()
	{
		FileSearch search(root);
		std::vector<std::string> files = search.filesPath_search();

		Reader reader(files);
		reader.read_file(m_);

		Writer writer(reader.getData());
		writer.write_to_file();
	}
	
private:
	std::string root;
	mode m_;
};