#define _CRT_SECURE_NO_WARNINGS // for ctime
#include "Reporter.h"
#include "ThreadPool.h"
#include <fstream>
#include <time.h>
#include <filesystem>
#include <ctime>

void Reporter::setReport(LinesResult result)
{
	std::ofstream outputReport;

	outputReport.open(filePath);

	if(!outputReport.is_open() || outputReport.fail())
		throw std::runtime_error("Could not open file: " + filePath);

	ThreadPool t;
	time_t timestamp;
	time(&timestamp);

	outputReport << "--------------------RESULT--------------------\n";
	outputReport << "ERROR FILES      : " << result.error_files << "\n";
	outputReport << "PROCESSED FILES  : " << result.processed_files << "\n";
	outputReport << "BLANK LINES      : " << result.blank_lines <<"\n";
	outputReport << "COMMENT LINES    : " << result.comment_lines << "\n";
	outputReport << "CODE LINES       : " << result.code_lines << "\n";
	outputReport << "TIME             : " << result.timeMS << " ms\n";
	outputReport << "USED THREADS     : " << t.getThreadCount() << "\n\n";
	outputReport << "LAST TIME REPORT : " << ctime(&timestamp)<<"\n";


	outputReport.close();
	
}