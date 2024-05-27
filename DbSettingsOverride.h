#pragma once

#include <string>
#include <set>
#include <map>
#include <stack>
#include <string>
#include <sstream>
#include "Utils.h"
#include "SqlRow.h"
#include "StoredObject.h"
#include "StringParameter.h"
#include "IntParameter.h"
#include "ParameterBase.h"
/*
inline static std::string UPDATE_DELAY_STR = "updatedelay";
inline static std::string USER_DELAY_STR = "userdelay";
inline static std::string COMBAT_TIMER_STR = "combattimer";
inline static std::string ASSIST_RANGE_STR = "assistrange";
inline static std::string HEAL_RANGE_STR = "healrange";
inline static std::string REZ_RANGE_STR = "rezrange";

inline static std::string COMBAT_TIMER_ENABLED_STR = "combattimerenabled";
inline static std::string FAKE_COMBAT_STR = "fakecombat";

inline static std::string OFF_STR = "off";
inline static std::string ON_STR = "on";

inline static std::string ROLE_STR = "role";
inline static std::string DPS_STR = "dps";
inline static std::string TANK_STR = "tank";
inline static std::string HEALER_STR = "healer";

inline static std::string MEZ_STR = "mez";
inline static std::string TARGETEDAE_STR = "targetae";
inline static std::string PBAOE_STR = "pbae";
*/
class DbSettingsOverride : public StoredObject {
private:
	std::string _Name = "";
	std::string _Value = "";
	int _IntegerValue = -1;
	int _CharacterID = -1;
	bool _AsString = false;
public:
	DbSettingsOverride() : StoredObject() {}

	DbSettingsOverride(
		std::string aName,
		std::string aValue,
		int aCharacterID,
		bool aAsString
	) : StoredObject() {
		_Name = aName;
		_Value = aValue;
		_CharacterID = aCharacterID;
		_AsString = aAsString;

		_Status = StoredStatus::INSERTED;
	}

	DbSettingsOverride(SqlRow aRow) : StoredObject() {
		_Name = aRow.GetValue("Name").AsString();
		_Value = aRow.GetValue("Value").AsString();
		_IntegerValue = aRow.GetValue("Value").AsInt();
		_CharacterID = aRow.GetValue("CharacterID").AsInt();
		_AsString = aRow.GetValue("ValueType").AsString().compare("S") == 0;
		_IsLoaded = true;
	}

	std::string GetInsertSql() override {
		return
			"	INSERT INTO SettingsOverride (Value, Name, CharacterID) \
				VALUES (?, ?, ?) ";
	}

	std::string GetUpdateSql() override {
		return
			"	UPDATE SettingsOverride \
			 	SET \
					Value = ? \
				WHERE \
					Name = ? \
					AND CharacterID = ? ";
	}

	std::vector<ParameterBase*> GetInsertParams() override {
		std::vector<ParameterBase*> lResult;

		lResult.push_back(new StringParameter(_Value));
		lResult.push_back(new StringParameter(_Name));
		lResult.push_back(new IntParameter(_CharacterID));

		return lResult;
	}

	std::vector<ParameterBase*> GetUpdateParams() override {
		return GetInsertParams();
	}

	void SetString(std::string aValue) {
		_Value = aValue;
	}

	void SetInt(std::string aValue) {
		_Value = aValue;
		_IntegerValue = Utils::StrToInt(aValue.c_str(), - 1);
	}

	void SetValue(std::string aValue) {
		if (_AsString) {
			SetString(aValue);
		}
		else {
			SetInt(aValue);
		}
	}

	std::string GetValue() {
		return _Value;
	}

	int GetValueAsInt() {
		return _IntegerValue;
	}

	std::string GetName() {
		return _Name;
	}
};