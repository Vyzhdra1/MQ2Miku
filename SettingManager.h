#pragma once
#include <set>
#include <map>
#include <stack>
#include <string>
#include "Utils.h"
#include "GlobalKeys.h"
#include "DbManager.h"
#include "DbSettings.h"
#include "SettingsContext.h"
#include "DbSettingsOverride.h"

class SettingManager {
private:
	inline static SettingManager* _Manager = 0;
	//std::map<std::string, DbSettings*> * _Settings;
	//std::map<std::string, DbSettingsOverride*> _SettingsOverrides;

	bool HasOverrideSetting(std::string aSetting) {
		return DbManager::Get()->GetSettingsOverrideContext()->HasSetting(aSetting);
	}

public:
	SettingManager() {
		//_Settings = DbManager::Get()->GetSettingsContext()->GetSettings();
		//SetDefaultSettings();
	}

	void UpdateSetting(std::string aSetting, std::string aValue) {
		if (!HasSetting(aSetting)) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No setting found: ", aSetting);
			return;
		}

		if (!DbManager::Get()->GetSettingsContext()->GetSetting(aSetting)->Validate(aValue)) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Invalid setting value: ", aValue);
		}
		
		DbSettingsOverride* lOverride = DbManager::Get()->GetSettingsOverrideContext()->GetSetting(aSetting);
		if (!lOverride) {
			lOverride = new DbSettingsOverride(
				aSetting, 
				aValue,
				DbManager::Get()->GetSettingsOverrideContext()->GetCharacterID(),
				DbManager::Get()->GetSettingsContext()->GetSetting(aSetting)->IsString()
			);
			DbManager::Get()->GetSettingsOverrideContext()->Add(aSetting, lOverride);
		}
		else {
			lOverride->SetValue(aValue);
		}

		if (CanOverridePersist(aSetting)) {
			DbManager::Get()->Save(lOverride);
		}

		Utils::MikuEcho(Utils::SUCCESS_COLOR, aSetting + " updated: ", aValue);
	}

	bool IsValueMatched(std::string aKey, std::string aValue) {
		return !GetValue(aKey).compare(aValue);
	}

	bool HasSetting(std::string aSetting) {
		return DbManager::Get()->GetSettingsContext()->HasSetting(aSetting);
	}

	bool CanOverridePersist(std::string aSetting) {
		if (!HasSetting(aSetting)) return false;
		return DbManager::Get()->GetSettingsContext()->GetSetting(aSetting)->CanOverridePersist();
	}

	std::string GetValue(std::string aKey) {
		if (HasOverrideSetting(aKey)) {
			return DbManager::Get()->GetSettingsOverrideContext()->GetSetting(aKey)->GetValue();
		}
		else if (HasSetting(aKey)) {
			return DbManager::Get()->GetSettingsContext()->GetSetting(aKey)->GetValue();
		}
		return "";
	}

	int GetInt(std::string aKey) {
		if (HasOverrideSetting(aKey)) {
			return DbManager::Get()->GetSettingsOverrideContext()->GetSetting(aKey)->GetValueAsInt();
		}
		else if (HasSetting(aKey)) {
			return DbManager::Get()->GetSettingsContext()->GetSetting(aKey)->GetValueAsInt();
		}
		return -1;
	}

	int GetUpdateDelay() {
		return GetInt(UPDATE_DELAY_STR);
	}

	int GetCombatTimer() {
		return GetInt(COMBAT_TIMER_STR);
	}

	int GetAssistRange() {
		return GetInt(ASSIST_RANGE_STR);
	}

	int GetHealRange() {
		return GetInt(HEAL_RANGE_STR);
	}

	int GetRezRange() {
		return GetInt(REZ_RANGE_STR);
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
		Utils::MikuEcho(Utils::BLUE_COLOR, "Total Settings: ", (int)DbManager::Get()->GetSettingsContext()->GetSettings()->size());
		std::map<std::string, DbSettings*>::iterator lIterator = DbManager::Get()->GetSettingsContext()->GetSettings()->begin();
		while (lIterator != DbManager::Get()->GetSettingsContext()->GetSettings()->end()) {
			std::string lKey = lIterator->first;
			std::string lValue = GetValue(lKey);

			Utils::MikuEcho(Utils::BLUE_COLOR, lKey, lValue);
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
