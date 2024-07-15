#pragma once
#include <string>
#include "SqlRow.h"
#include "StoredObject.h"
#include "ParameterBase.h"
#include "StringParameter.h"
#include "IntParameter.h"

class DbSpellSetLink : public StoredObject
{
private:
	std::string _SpellKey = "";
	std::string _ClassKey = "";
	std::string _Name = "";
	int _GemIndex = -1;
public:
	DbSpellSetLink() : StoredObject() { }

	DbSpellSetLink(std::string aClassKey) : StoredObject() {
		_ClassKey = aClassKey;

		_Status = StoredStatus::INSERTED;
	}

	DbSpellSetLink(SqlRow aRow) : StoredObject() {
		LoadFromDb(aRow);
		_IsLoaded = true;
	}

	void LoadFromDb(SqlRow aRow) override {
		_SpellKey = aRow.GetValue("SpellKey").AsString();
		_Name = aRow.GetValue("Name").AsString();
		_GemIndex = aRow.GetValue("GemIndex").AsInt();

		_IsLoaded = true;
		_Status = StoredStatus::NOCHANGE;
	}

	std::string GetSpellKey() {
		return _SpellKey;
	}

	std::string GetClassKey() {
		return _ClassKey;
	}

	std::string GetName() {
		return _Name;
	}

	int GetGemIndex() {
		return _GemIndex;
	}
};