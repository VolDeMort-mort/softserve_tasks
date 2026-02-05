#pragma once
#include "Collector.h"
class Reporter
{
private:
	std::string filePath;
public:
	Reporter(const std::string& file) : filePath(file){}
	void setReport(LinesResult result);
};

