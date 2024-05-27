#pragma once
#include "DbConnection.h"
#include "sqlite3.h"

class ParameterBase
{
public:
	ParameterBase() {}
	~ParameterBase() {}

	virtual int Bind(sqlite3_stmt* lStatement, int aIndex) = 0;
};



