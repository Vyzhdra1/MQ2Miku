#pragma once
#include "DbConnection.h"
#include <string>
#include "DbManager.h"
#include "SettingsContext.h"

class StoredObject {
private:  
	DbConnection* _Connection = 0;
protected:
	bool _IsLoaded = false;
	enum StoredStatus { INSERTED, DELETED, UPDATED, NOCHANGE };
	StoredStatus _Status = StoredStatus::NOCHANGE;

	void Fetch(DbConnection* aConnection) {
		Query lQuery;
		lQuery.SetSql(GetFetchSql());
		lQuery.AddParameters(GetFetchParams());

		aConnection->QueryDatabasePrepare(&lQuery);

		for (SqlRow lRow : lQuery.GetResult().GetRows()) {
			LoadFromDb(lRow);
		}
	}
public:
	StoredObject() {}
	~StoredObject() {}

	virtual bool IsLoaded() {
		return _IsLoaded;
	}

	void SetConnection(DbConnection* aConnection) {
		_Connection = aConnection;
	}

	void Save(DbConnection * aConnection) {
		Query lQuery;

		if (_Status == StoredStatus::NOCHANGE) {
			lQuery.SetSql(GetUpdateSql());
			lQuery.AddParameters(GetUpdateParams());
		}
		else if(_Status == StoredStatus::INSERTED) {
			lQuery.SetSql(GetInsertSql());
			lQuery.AddParameters(GetInsertParams());

		} else {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to Save");

		}

		aConnection->QueryDatabasePrepare(&lQuery);
	}

	void Load(DbConnection* aConnection) {
		Fetch(aConnection);

		if (IsLoaded()) return;

		Save(aConnection);
		Fetch(aConnection);

		if (!IsLoaded()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to load data");
		}
	}

	virtual void LoadFromDb(SqlRow aRow) {}

	virtual std::string GetInsertSql() {
		return "";
	}

	virtual std::string GetUpdateSql() {
		return "";
	}

	virtual std::string GetFetchSql() {
		return "";
	}

	virtual std::vector<ParameterBase*> GetInsertParams() {
		std::vector<ParameterBase*> lResult;
		return lResult;
	}

	virtual std::vector<ParameterBase*> GetUpdateParams() {
		std::vector<ParameterBase*> lResult;
		return lResult;
	}

	virtual std::vector<ParameterBase*> GetFetchParams() {
		std::vector<ParameterBase*> lResult;
		return lResult;
	}
};