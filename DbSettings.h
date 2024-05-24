#pragma once

#include <string>
#include <set>
#include <map>
#include <stack>
#include <string>

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

class DbSetting {
public:
	std::string _Name = "";
	bool _AsString = true;
	std::string _Value = "";
	std::set<std::string> _ValidStringValues;
	int _IntegerValue = -1;
	int _Min = -1;
	int _Max = -1;
	std::function<bool(std::string aValue)> _ValidationFunction = 0;

	MikuSetting() {}

	MikuSetting(std::string aName, std::string aValue, std::set<std::string> aValidStringValues) {
		_Name = aName;
		_Value = aValue;
		_ValidStringValues = aValidStringValues;
		_AsString = true;
	}

	MikuSetting(std::string aName, int aValue, int aMin, int aMax) {
		_Name = aName;
		_IntegerValue = aValue;
		_Min = aMin;
		_Max = aMax;
		_Value = std::to_string(aValue);
		_AsString = false;
	}

	void SetString(std::string aValue) {
		if (!ValidateString(aValue)) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Invalid setting value: ", aValue);
			return;
		}

		_Value = aValue;
		Utils::MikuEcho(Utils::SUCCESS_COLOR, _Name + " updated: ", _Value);
	}

	void SetInt(std::string aValue) {
		if (!ValidateGenericInteger(aValue)) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Invalid setting value: ", aValue);
			return;
		}

		_Value = aValue;
		_IntegerValue = Utils::StrToInt(aValue.c_str(), _Min - 1);
		Utils::MikuEcho(Utils::SUCCESS_COLOR, _Name + " updated: ", _Value);
	}

	void SetValue(std::string aValue) {
		if (_AsString) {
			SetString(aValue);
		}
		else {
			SetInt(aValue);
		}
	}

	bool ValidateGenericInteger(std::string aValue) {
		int lIntValue = Utils::StrToInt(aValue.c_str(), _Min - 1) != -1;
		return (lIntValue >= _Min) && (lIntValue <= _Max);
	}

	bool ValidateString(std::string aValue) {
		return _ValidStringValues.find(aValue) != _ValidStringValues.end();
	}

	std::string GetValue() {
		return _Value;
	}

	int GetValueAsInt() {
		return _IntegerValue;
	}
};