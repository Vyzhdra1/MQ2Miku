#pragma once
#include <string>
#include "SqlRow.h"
#include "StoredObject.h"

class DbAlternateAbilities : public StoredObject
{
private:
	std::string _AAKey = "";
	std::string _ClassKey = "";
	int _ActCode = -1;
	bool _IsBuff = false;
public:
	DbAlternateAbilities() : StoredObject() { }

	DbAlternateAbilities(std::string aClassKey) : StoredObject() {
		_ClassKey = aClassKey;

		_Status = StoredStatus::INSERTED;
	}

	DbAlternateAbilities(SqlRow aRow) : StoredObject() {
		LoadFromDb(aRow);
		_IsLoaded = true;
	}

	void LoadFromDb(SqlRow aRow) override {
		_AAKey = aRow.GetValue("AAKey").AsString();
		_ActCode = aRow.GetValue("ActCode").AsInt();
		_IsBuff = aRow.GetValue("IsBuff").AsString().compare("Y") == 0;

		_IsLoaded = true;
		_Status = StoredStatus::NOCHANGE;
	}

	std::string GetAAKey() {
		return _AAKey;
	}

	std::string GetClassKey() {
		return _ClassKey;
	}

	int GetActCode() {
		return _ActCode;
	}

	bool GetIsBuff() {
		return _IsBuff;
	}
};