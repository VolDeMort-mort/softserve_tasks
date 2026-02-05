#pragma once
#include "pch.h"

#include "ProjectResult.h"
#include "FileResult.h"

class AbstractResultWriter
{
public:
	AbstractResultWriter() = default;

	virtual void write(const ProjectResult& prResult) = 0;

	virtual ~AbstractResultWriter() = default;
};

