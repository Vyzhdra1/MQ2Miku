#pragma once
#include <string>
#include <vector>
#include "SqlRow.h"
#include "sqlite3.h"

class SqlResult
{
private:
	std::vector<SqlRow> _Results;
public:
	SqlResult() { }

	int ResultCount() { 
		return _Results.size();
	}

	void AddRow(sqlite3_stmt* lStatement) {
		int lColumnCount = sqlite3_column_count(lStatement);

		SqlRow lRow;
		for (int i = 0; i < lColumnCount; i++) {
			std::string lColumnName = sqlite3_column_name(lStatement, i);
			std::string lValue = "";
			if (sqlite3_column_type(lStatement, i) != SQLITE_NULL) {
				lValue = std::string(reinterpret_cast<const char*>(sqlite3_column_text(lStatement, i)));
			}
			lRow.AddValue(lColumnName, lValue);
		}
		_Results.push_back(lRow);
	}

	std::vector<SqlRow> GetRows() {
		return _Results;
	}
};
