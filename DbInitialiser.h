#pragma once

#include "MikuPlayer.h"
#include "DbManager.h"
#include "SettingManager.h"
#include "Utils.h"

class DbInitialiser
{
private:
	static void LoadCharacter(DbManager* aDbManager, MikuPlayer* aPlayer) {
		DbCharacter * lCharacter = new DbCharacter(
			aPlayer->GetName(), aPlayer->GetLevel(), aPlayer->GetServer(), aPlayer->GetClassName(), aPlayer->GetAccountName());
		aDbManager->Load(lCharacter);
		aDbManager->AssignDbCharacter(lCharacter);
	}

	static void LoadSettingsOverride(DbManager* aDbManager, DbCharacter* aCharacter) {
		aDbManager->GetSettingsOverrideContext()->SetParameters(aCharacter->GetCharacterID());
		aDbManager->GetSettingsOverrideContext()->LoadObjects();

		if (!aDbManager->GetSettingsOverrideContext()->HasSetting(ROLE_STR)) {
			SettingManager::Get()->UpdateSetting(ROLE_STR, aCharacter->GetDefaultRole());
		}
	}

	static void LoadSettings(DbManager* aDbManager, MikuPlayer* aPlayer) {
		aDbManager->GetSettingsContext()->SetParameters(aPlayer->GetClassName());
		aDbManager->GetSettingsContext()->LoadObjects();
	}

public:
	static void Load(DbManager* aDbManager, MikuPlayer* aPlayer) {
		LoadSettings(aDbManager, aPlayer);
		LoadCharacter(aDbManager, aPlayer);

		DbCharacter* lCharacter = aDbManager->GetCharacter();
		if (!lCharacter->IsLoaded()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to load character data");
			return;
		}

		LoadSettingsOverride(aDbManager, lCharacter);
	}
};