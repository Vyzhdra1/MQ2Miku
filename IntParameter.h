#pragma once
#include "ParameterBase.h"

class IntParameter : public ParameterBase
{
private:
	int _Value;
public:
	IntParameter(int aValue) : ParameterBase() {
		_Value = aValue;
	}

	virtual int Bind(sqlite3_stmt* lStatement, int aIndex) override {
		return sqlite3_bind_int(lStatement, aIndex, _Value);
	}
};



