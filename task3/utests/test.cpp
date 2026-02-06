#include <Scaner.h>
#include <gtest/gtest.h>


TEST(FileSearchTest, PathSearch)
{
	FileSearch search_test("D:\\softserve\\task1_strList");
	auto result = search_test.filesPath_search();
	EXPECT_EQ(result.size(), 4);

}


TEST(FileSearchTest, FileExtensionsTest)
{
	FileSearch search_test("D:\\softserve\\task1_strList");
	auto result = search_test.filesPath_search();
	bool cpp = false;
	bool h = false;
	bool hpp = false;
	bool c = false;
	for (const auto& file : result)
	{
		std::string ext = file.substr(file.find_last_of(".") + 1);
		if (ext == "cpp") cpp = true;
		if (ext == "h" && file.find(".hpp") == std::string::npos) h = true;
		if (ext == "c") c = true;
		if (ext == "hpp") hpp = true;
		EXPECT_TRUE(ext == "cpp" || ext == "h" || ext == "c" || ext == "hpp");
	}

}


TEST(ReaderTest, CountsLinesCorrectly) {
	FileSearch search_test("D:\\testdata_single");
	auto result = search_test.filesPath_search();
	Reader reader(result);
	reader.read_file(mode::etype_single);
	auto data = reader.getData();

	EXPECT_EQ(data->total_files, 1);
	EXPECT_EQ(data->blank_lines, 6);
	EXPECT_EQ(data->comment_lines, 7);
	EXPECT_EQ(data->code_lines, 6);
}



TEST(ReaderTest, MultipleFiles) {
	FileSearch search_test("D:\\try_project");
	auto result = search_test.filesPath_search();
	Reader reader(result);
	reader.read_file(mode::etype_single);
	auto data = reader.getData();

	EXPECT_EQ(data->total_files, 3);
	EXPECT_EQ(data->code_lines, 0); 
}


TEST(ReaderTest, MultithreadedMatchesSingleThreaded) {
	FileSearch search_test("D:\\try_project2");
	auto result = search_test.filesPath_search();
	Reader single(result);
	single.read_file(mode::etype_single);
	auto single_data = single.getData();

	Reader multi(result);
	multi.read_file(mode::etype_multi);
	auto multi_data = multi.getData();

	EXPECT_EQ(single_data->total_files, multi_data->total_files);
	EXPECT_EQ(single_data->blank_lines, multi_data->blank_lines);
	EXPECT_EQ(single_data->comment_lines, multi_data->comment_lines);
	EXPECT_EQ(single_data->code_lines, multi_data->code_lines);
}


TEST(OutputTest, SaveToFile)
{
	FileSearch search_test("D:\\try_project2");
	auto result = search_test.filesPath_search();
	Reader multi(result);
	multi.read_file(mode::etype_multi);
	auto multi_data = multi.getData();
	Writer writer(multi_data);
	writer.write_to_file();
	std::ifstream file("Statistics.txt");
	EXPECT_TRUE(file.is_open());
	std::string line;
	bool found_total_files = false;
	while (std::getline(file, line)) {
		if (line.find("Total files:") != std::string::npos)
			found_total_files = true;
	}
	EXPECT_TRUE(found_total_files);
}


TEST(BigProjectTest, StressTestDoesNotCrash)
{
	FileSearch search("D:\\try_project5");
	auto files = search.filesPath_search();

	Reader reader(files);
	EXPECT_NO_THROW(
		reader.read_file(mode::etype_multi)// Files: 6571, multi_time: 20.4516s, single_time: 62.6815s
	);
}

