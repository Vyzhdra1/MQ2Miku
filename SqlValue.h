#pragma once
#include <string>
#include "Utils.h"

class SqlValue
{
private:
	std::string _Value;
public:
	SqlValue() {}

	SqlValue(std::string aValue) {
		_Value = aValue;
	}

	std::string AsString() {
		return _Value;
	}

	int AsInt() {
		return Utils::StrToInt(_Value.c_str(), -1);
	}

	bool AsBool() {
		return _Value.compare("Y") == 0;
	}
};

