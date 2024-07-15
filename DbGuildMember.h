#pragma once
#include <string>
#include "SqlRow.h"
#include "StoredObject.h"

class DbGuildMember : public StoredObject
{
private:
	SqlRow _Data;
public:
	DbGuildMember() : StoredObject() { }

	DbGuildMember(SqlRow aRow) : StoredObject() {
		LoadFromDb(aRow);
		_IsLoaded = true;
	}

	void LoadFromDb(SqlRow aRow) override {
		_IsLoaded = true;
		_Status = StoredStatus::NOCHANGE;
	}

	std::string Name() {
		return _Data.GetValue("Name").AsString();
	}

	std::string ClassKey() {
		return _Data.GetValue("ClassKey").AsString();
	}

	int CharacterID() {
		return _Data.GetValue("CharacterID").AsInt();
	}

	std::string Role() {
		return _Data.GetValue("Role").AsString();
	}
};