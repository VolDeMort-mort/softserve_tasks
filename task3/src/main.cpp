#include "Scaner.h"


int main()
{
	std::string rootPath = { "D:\\try_project5" };
	Scaner scaner(rootPath, mode::etype_multi);
	scaner.run();
}