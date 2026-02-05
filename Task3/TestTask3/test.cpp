#include "pch.h"
#include "../Task 3/Searcher.h"
#include "../Task 3/Counter.h" 
#include "../Task 3/Reporter.h"
#include "../Task 3/Collector.h"
#include "../Task 3/ThreadPool.h"

TEST(Searcher, searchingTesting)
{
	Searcher s;

	// current project path check
	std::string path = "D:\\Programming\\source\\repos\\Task 3\\Task 3"; // ~ 380 000 lines for second
	EXPECT_NO_THROW(s.startSearch(path));

	// huge project path check
	std::string path_huge = "D:\\Programming\\openCV\\modules"; // ~ 3.5 million lines for second
	EXPECT_NO_THROW(s.startSearch(path_huge));

	// incorrect path check
	std::string error_path = "D:\\incorrect";
	EXPECT_THROW(s.startSearch(error_path), std::runtime_error);
	
	//extension check
	std::filesystem::path p = "correct.cpp";
	std::filesystem::path error_p = "incorrect.txt";

	EXPECT_TRUE(s.checkExtension(p.extension()));
	EXPECT_FALSE(s.checkExtension(error_p.extension()));
}
TEST(Counter, counterTesting)
{
	Counter c;
	
	//open file check
	std::string str = "D:\\Programming\\source\\repos\\Task 3\\Task 3\\Collector.cpp";
	std::string strIncorrect = "D:\\Collector.txt";
	
	const std::filesystem::directory_entry path(str);
	const std::filesystem::directory_entry pathIncorrect(strIncorrect);

	LinesResult res = c.readFile(path);
	LinesResult res_error = c.readFile(pathIncorrect);

	EXPECT_EQ(res.error_files,0);
	EXPECT_EQ(res_error.error_files, 1);

	// correct result check
	Collector coll;

	std::string strCheckLines = "D:\\Programming\\source\\repos\\Task 3\\Task 3\\Collector.h";
	auto result = c.readFile(std::filesystem::directory_entry(strCheckLines));

	coll.addFileStats(result);

	auto newResult = coll.getResult();

	EXPECT_EQ(newResult.blank_lines, 2);
	EXPECT_EQ(newResult.code_lines, 20);
	EXPECT_EQ(newResult.comment_lines, 0);
	EXPECT_EQ(newResult.processed_files, 1);
}
TEST(Reporter, reporterTesting)
{
	//report output check
	LinesResult result = { 12,33,7,2,29 };

	Reporter r("result.txt");
	EXPECT_NO_THROW(r.setReport(result));

	Reporter incorect_report("E:\\no_result.txt");
	EXPECT_THROW(incorect_report.setReport(result),std::runtime_error);

}
TEST(ThreadPool, threadPoolTesting)
{
	ThreadPool t;
	Counter c;
	Collector coll;
	//empty queue check
	EXPECT_THROW(t.startThreads(c, coll), std::runtime_error);
}