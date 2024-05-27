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
class DbSettings: public StoredObject {
private:
	std::string _Name = "";
	bool _AsString = true;
	std::string _Value = "";
	std::set<std::string> _ValidStringValues;
	int _IntegerValue = -1;
	int _Min = -1;
	int _Max = -1;
	std::function<bool(std::string aValue)> _ValidationFunction = 0;

	void InitValidStringValues(std::string aValues) {
		if (aValues.empty()) return;

		std::istringstream lStringStream(aValues);
		std::string lString;
		while (getline(lStringStream, lString, ',')) {
			_ValidStringValues.insert(lString);
		}
	}
public:
	DbSettings() : StoredObject() {}

	DbSettings(SqlRow aRow) : StoredObject() {
		_Name = aRow.GetValue("Name").AsString();
		_Value = aRow.GetValue("Value").AsString();
		_AsString = !aRow.GetValue("ValueType").AsString().compare("S");

		if (_AsString) {
			InitValidStringValues(aRow.GetValue("ValidStrings").AsString());
		}
		else {
			_IntegerValue = Utils::StrToInt(_Value.c_str(), -1);
			_Min = aRow.GetValue("MinValue").AsInt();
			_Max = aRow.GetValue("MaxValue").AsInt();
		}
		_IsLoaded = true;
	}

	bool ValidateGenericInteger(std::string aValue) {
		int lIntValue = Utils::StrToInt(aValue.c_str(), _Min - 1) != -1;
		return (lIntValue >= _Min) && (lIntValue <= _Max);
	}

	bool ValidateString(std::string aValue) {
		return _ValidStringValues.find(aValue) != _ValidStringValues.end();
	}

	bool Validate(std::string aNewValue) {
		if (_AsString) {
			return ValidateString(aNewValue);
		}
		else {
			return ValidateGenericInteger(aNewValue);
		}
	}

	bool IsString() {
		return _AsString;
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