#pragma once
#include <set>
#include <map>
#include <stack>
#include <string>
#include "Utils.h"
#include "GlobalKeys.h"

class MikuSetting {
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
		int lIntValue = Utils::StrToInt(aValue.c_str(), _Min -1) != -1;
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

class SettingManager {
private:
	inline static SettingManager* _Manager = 0;
	std::map<std::string, MikuSetting> _Settings;

	void SetDefaultSettings() {
		_Settings.clear();
		_Settings[ROLE_STR] = MikuSetting(ROLE_STR, DPS_STR, { DPS_STR, HEALER_STR, TANK_STR });
		_Settings[MEZ_STR] = MikuSetting(MEZ_STR, TARGETEDAE_STR, { TARGETEDAE_STR, PBAOE_STR, OFF_STR });
		_Settings[COMBAT_TIMER_ENABLED_STR] = MikuSetting(COMBAT_TIMER_ENABLED_STR, ON_STR, { ON_STR, OFF_STR });
		_Settings[FAKE_COMBAT_STR] = MikuSetting(FAKE_COMBAT_STR, OFF_STR, { ON_STR, OFF_STR });
		_Settings[UPDATE_DELAY_STR] = MikuSetting(UPDATE_DELAY_STR, 600, 0, 100000);
		_Settings[COMBAT_TIMER_STR] = MikuSetting(COMBAT_TIMER_STR, 8000, 0, 100000);
		_Settings[ASSIST_RANGE_STR] = MikuSetting(ASSIST_RANGE_STR, 200, 0, 100000);
		_Settings[USER_DELAY_STR] = MikuSetting(USER_DELAY_STR, 8000, 0, 100000);
		_Settings[HEAL_RANGE_STR] = MikuSetting(HEAL_RANGE_STR, 250, 0, 300);
		_Settings[REZ_RANGE_STR] = MikuSetting(REZ_RANGE_STR, 100, 0, 300);
	}

	bool HasSetting(std::string aSetting) {
		return _Settings.find(aSetting) != _Settings.end();
	}
public:
	SettingManager() {
		SetDefaultSettings();
	}

	void UpdateSetting(std::string aSetting, std::string aValue) {
		if (_Settings.find(aSetting) == _Settings.end()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No setting found: ", aSetting);
			return;
		}

		_Settings[aSetting].SetValue(aValue);
	}

	bool IsValueMatched(std::string aKey, std::string aValue) {
		if (!HasSetting(aKey)) return false;

		return !_Settings[aKey].GetValue().compare(aValue);
	}

	int GetUpdateDelay() {
		return _Settings[UPDATE_DELAY_STR].GetValueAsInt();
	}

	int GetCombatTimer() {
		return _Settings[COMBAT_TIMER_STR].GetValueAsInt();
	}

	int GetAssistRange() {
		return _Settings[ASSIST_RANGE_STR].GetValueAsInt();
	}

	int GetHealRange() {
		return GetInt(HEAL_RANGE_STR);
	}

	int GetRezRange() {
		return GetInt(REZ_RANGE_STR);
	}

	int GetInt(std::string aKey) {
		return _Settings[aKey].GetValueAsInt();
	}

	void ParseCommand(char*  aInput) {
		std::vector<std::string> lParams = Utils::GetParamList(aInput);
		
		std::vector<std::string>::iterator lIterator = lParams.begin();

		if (!lParams.size()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Two parameters required. Usage: /mikuset [Setting] [Value]");
			return;
		}

		std::string lSetting = *lIterator;

		if (lParams.size() == 1) {
			if (!lSetting.compare("list")) {
				List();
				return;
			}
			return;
		}

		if (lParams.size() != 2) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Two parameters required. Usage: ", "/mikuset[Setting][Value]");
			return;
		}

		if (!HasSetting(lSetting)) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not find setting: ", lSetting);
			return;
		}

		lIterator++;

		UpdateSetting(lSetting, *lIterator);
	}

	void List() {
		Utils::MikuEcho(Utils::BLUE_COLOR, "Total Settings: ", (int)_Settings.size());
		std::map<std::string, MikuSetting>::iterator lIterator = _Settings.begin();
		while (lIterator != _Settings.end()) {
			std::string lKey = lIterator->first;
			MikuSetting lSetting = lIterator->second;
			Utils::MikuEcho(Utils::BLUE_COLOR, lKey, lSetting.GetValue());
			lIterator++;
		}
	}

	static void Deinit() {
		if (_Manager) {
			delete _Manager;
			_Manager = 0;
		}
	}

	static SettingManager * Get() {
		if (!_Manager) {
			_Manager = new SettingManager();
		}
		return _Manager;
	}
};
