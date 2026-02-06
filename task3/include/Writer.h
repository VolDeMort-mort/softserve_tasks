#include <fstream>
#include <utility> 


struct statistics;

class Writer
{
public:
	explicit Writer(std::shared_ptr<statistics> data);
	void write_to_file();

private:
	std::shared_ptr<statistics> data_to_write;
};