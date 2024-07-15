#pragma once
#include <string>
#include "SqlRow.h"
#include "StoredObject.h"

class DbGroup : public StoredObject
{
private:
	SqlRow _Data;
public:
	DbGroup() : StoredObject() { }

	DbGroup(SqlRow aRow) : StoredObject() {
		LoadFromDb(aRow);
		_IsLoaded = true;
	}

	void LoadFromDb(SqlRow aRow) override {
		_IsLoaded = true;
		_Status = StoredStatus::NOCHANGE;
	}

	std::string GroupKey() {
		return _Data.GetValue("GroupKey").AsString();;
	}

	std::string Name() {
		return _Data.GetValue("Name").AsString();;
	}

	bool IsLeader() {
		return _Data.GetValue("IsLeader").AsBool();;
	}

	int GroupID() {
		return _Data.GetValue("GroupID").AsInt();;
	}

	int GroupIndex() {
		return _Data.GetValue("GroupIndex").AsInt();;
	}
};