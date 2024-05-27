#pragma once
#include <string>
#include <map>
#include "SqlRow.h"
#include "SqlValue.h"

class SqlRow
{
private:
	std::map<std::string, SqlValue> _RowData;
public:
	SqlRow() {}

	void AddValue(std::string aColumn, std::string aValue) {
		_RowData[aColumn] = SqlValue(aValue);
	}

	SqlValue GetValue(std::string aColumn) {
		return _RowData[aColumn];
	}
};
