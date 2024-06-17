#pragma once
#include <string>
#include "SqlRow.h"
#include "StoredObject.h"
#include "ParameterBase.h"
#include "StringParameter.h"
#include "IntParameter.h"

class DbSpell : public StoredObject
{
private:
	std::string _SpellKey = "";
	std::string _ClassKey = "";
	std::string _Description = "";
	int _Target = -1;
	bool _IsCombatAbility = false;
	bool _HasRecourse = false;
	int _LevelOffset = -1;
	int _ExactLevel = -1;
	int _Timer = -1;
	int _SPA1 = -1;
	int _SPA2 = -1;
	int _SPA3 = -1;
	bool _IsBuff = false;
public:
	DbSpell() : StoredObject() { }

	DbSpell(std::string aClassKey) : StoredObject() {
		_ClassKey = aClassKey;

		_Status = StoredStatus::INSERTED;
	}

	DbSpell(SqlRow aRow) : StoredObject() {
		LoadFromDb(aRow);
		_IsLoaded = true;
	}

	void LoadFromDb(SqlRow aRow) override {
		_SpellKey = aRow.GetValue("SpellKey").AsString();
		_Description = aRow.GetValue("Description").AsString();
		_Target = aRow.GetValue("Target").AsInt();

		_LevelOffset = aRow.GetValue("LevelOffset").AsInt();
		_ExactLevel = aRow.GetValue("ExactLevel").AsInt();
		_SPA1 = aRow.GetValue("SPA1").AsInt();
		_SPA2 = aRow.GetValue("SPA2").AsInt();
		_SPA3 = aRow.GetValue("SPA3").AsInt();

		_Timer = aRow.GetValue("Timer").AsInt();

		_IsCombatAbility = aRow.GetValue("IsCombatAbility").AsString().compare("Y") == 0;
		_HasRecourse = aRow.GetValue("HasRecourse").AsString().compare("Y") == 0;
		_IsBuff = aRow.GetValue("IsBuff").AsString().compare("Y") == 0;

		_IsLoaded = true;
		_Status = StoredStatus::NOCHANGE;
	}

	std::string GetSpellKey() {
		return _SpellKey;
	}

	std::string GetClassKey() {
		return _ClassKey;
	}

	std::string GetDescription() {
		return _Description;
	}

	int GetTarget() {
		return _Target;
	}

	bool GetIsCombatAbility() {
		return _IsCombatAbility;
	}

	bool GetHasRecourse() {
		return _HasRecourse;
	}

	int GetLevelOffset() {
		return _LevelOffset;
	}

	int GetExactLevel() {
		return _ExactLevel;
	}

	int GetTimer() {
		return _Timer;
	}

	int GetSPA1() {
		return _SPA1;
	}

	int GetSPA2() {
		return _SPA2;
	}

	int GetSPA3() {
		return _SPA3;
	}

	bool GetIsBuff() {
		return _IsBuff;
	}
};