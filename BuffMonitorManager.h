#pragma once
#include "BuffMonitor.h"

class BuffMonitorManager {
private:
	inline static BuffMonitorManager* _Manager = 0;
	BuffMonitor* _BuffMonitor = 0;
	std::map<std::string, MikuSetting> _Settings;

	void SetDefaultSettings() {
		_Settings.clear();
		_Settings[ROLE_STR] = MikuSetting(ROLE_STR, DPS_STR, { DPS_STR, HEALER_STR, TANK_STR });
		_Settings[MEZ_STR] = MikuSetting(MEZ_STR, TARGETEDAE_STR, { TARGETEDAE_STR, PBAOE_STR, OFF_STR });
		_Settings[COMBAT_TIMER_ENABLED_STR] = MikuSetting(COMBAT_TIMER_ENABLED_STR, ON_STR, { ON_STR, OFF_STR });
		_Settings[UPDATE_DELAY_STR] = MikuSetting(UPDATE_DELAY_STR, 600, 0, 100000);
		_Settings[COMBAT_TIMER_STR] = MikuSetting(COMBAT_TIMER_STR, 8000, 0, 100000);
		_Settings[ASSIST_RANGE_STR] = MikuSetting(ASSIST_RANGE_STR, 200, 0, 100000);
		_Settings[USER_DELAY_STR] = MikuSetting(USER_DELAY_STR, 8000, 0, 100000);
	}

	bool HasSetting(std::string aSetting) {
		return _Settings.find(aSetting) != _Settings.end();
	}
public:
	void SetBuffMonitor(BuffMonitor* aBuffMonitor) {
		_BuffMonitor = _BuffMonitor;
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

	int GetInt(std::string aKey) {
		return _Settings[aKey].GetValueAsInt();
	}

	void ParseCommand(char* aInput) {
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

	static void Deinit() {
		if (_Manager) {
			delete _Manager;
			_Manager = 0;
		}
	}

	static BuffMonitorManager* Get() {
		if (!_Manager) {
			_Manager = new BuffMonitorManager();
		}
		return _Manager;
	}
};

