#include "pch.h"
#include "../Task3/fileAnalyzer.h"

TEST(StatsA, TestName) {
	fileAnalyzer test;
	test.init(R"(C:\Users\demya\Desktop\test\test3)", true, nullptr, nullptr, [](fileStats stats, bool stat) {
		EXPECT_EQ(stats.filesFound, 5);
		EXPECT_EQ(stats.filesAnalyzed, 5);
		EXPECT_EQ(stats.lines, 61);
		EXPECT_EQ(stats.comments, 11);
		EXPECT_EQ(stats.blank, 32);
		EXPECT_EQ(stats.code, 15);
	});

	test.run();
}

TEST(StatsB, TestName) {

	fileAnalyzer test2;
	test2.init(R"(C:\Users\demya\Desktop\test)", true, nullptr, nullptr, [](fileStats stats, bool stat) {
		EXPECT_EQ(stats.filesFound, 12);
		EXPECT_EQ(stats.filesAnalyzed, 12);
		EXPECT_EQ(stats.lines, 89);
		EXPECT_EQ(stats.code, 21);
		EXPECT_EQ(stats.comments, 25);
		EXPECT_EQ(stats.blank, 40);
		});

	test2.run();
}