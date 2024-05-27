#pragma once
#include <string>
#include "ParameterBase.h"

class StringParameter: public ParameterBase
{
private:
	std::string _Value;
public:
	StringParameter(std::string aValue) : ParameterBase() {
		_Value = aValue;
	}

	virtual int Bind(sqlite3_stmt* lStatement, int aIndex) override {
		return sqlite3_bind_text(lStatement, aIndex, _Value.c_str(), strlen(_Value.c_str()), NULL);
	}
};



