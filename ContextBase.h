#pragma once
#include <vector>
#include "DbConnection.h"
#include "ParameterBase.h"
#include "SqlResult.h"

class ContextBase
{
protected:
	DbConnection* _Connection = 0;

	virtual std::string GetQueryString() = 0;
	virtual std::vector<ParameterBase*> GetParams() = 0;
	virtual void Clear() = 0;
	virtual void LoadDbObjects(SqlResult aResult) = 0;
public:
	ContextBase(DbConnection* aConnection) {
		_Connection = aConnection;
	}

	~ContextBase() {
		//Clear();
	}

	void LoadObjects() {
		Clear();

		Query lQuery;
		lQuery.SetSql(GetQueryString());
		lQuery.AddParameters(GetParams());

		_Connection->QueryDatabasePrepare(&lQuery);
		LoadDbObjects(lQuery.GetResult());
	}
};


