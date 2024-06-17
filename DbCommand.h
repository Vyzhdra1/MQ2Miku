#pragma once
#include <string>
#include "SqlRow.h"
#include "StoredObject.h"

class DbCommand : public StoredObject
{
private:
	std::string _CommandKey = "";
	std::string _CommandText = "";
public:
	DbCommand() : StoredObject() { }

	DbCommand(SqlRow aRow) : StoredObject() {
		LoadFromDb(aRow);
		_IsLoaded = true;
	}

	void LoadFromDb(SqlRow aRow) override {
		_CommandKey = aRow.GetValue("CommandKey").AsString();
		_CommandText = aRow.GetValue("CommandText").AsString();

		_IsLoaded = true;
		_Status = StoredStatus::NOCHANGE;
	}

	std::string GetCommandKey() {
		return _CommandKey;
	}

	std::string GetCommandText() {
		return _CommandText;
	}
};