#pragma once
#include <string>
#include "SqlResult.h"
#include "ParameterBase.h"
#include "sqlite3.h"

enum QueryMode { READ, WRITE };
class Query
{
private:
	std::string _SqlStr;
	std::vector<ParameterBase *> _Params;
	SqlResult _Result;
	QueryMode _Mode = QueryMode::READ;
public:

	Query() { }
	~Query() {
		for (ParameterBase* lParam : _Params) {
			delete lParam;
		}
		_Params.clear();
	}

	void SetMode(QueryMode aMode) {
		_Mode = aMode;
	}

	QueryMode GetMode() {
		return _Mode;
	}

	void AddResultRow(sqlite3_stmt* lStatement) {
		_Result.AddRow(lStatement);
	}

	void SetSql(std::string aSqlStr) {
		_SqlStr = aSqlStr;
	}

	std::string GetSql() {
		return _SqlStr;
	}

	SqlResult GetResult() {
		return _Result;
	}

	std::vector<ParameterBase *> GetParams() {
		return _Params;
	}

	void AddParameters(std::vector<ParameterBase *> aParams) {
		_Params.assign(aParams.begin(), aParams.end());
	}

	void AddParameter(ParameterBase * aParam) {
		_Params.push_back(aParam);
	}
};

