#pragma once

#include <vector>
#include <mq/Plugin.h>
#include "DbManager.h"
#include "SettingManager.h"
#include "DbAbilityLoader.h"
#include "Utils.h"

class DbLoader
{
private:
	static void LoadCharacter(DbManager* aDbManager) {
		DbCharacter * lCharacter = new DbCharacter(
			GetCharInfo()->Name, 
			GetPcProfile()->Level, 
			GetServerShortName(), 
			pEverQuest->GetClassThreeLetterCode(GetPcProfile()->Class), 
			GetLoginName());

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

	static void LoadSettings(DbManager* aDbManager) {
		aDbManager->GetSettingsContext()->SetParameters(pEverQuest->GetClassThreeLetterCode(GetPcProfile()->Class));
		aDbManager->GetSettingsContext()->LoadObjects();
	}

public:
	static void Load() {
		DbManager* lDbManager = DbManager::Get();
		LoadSettings(lDbManager);
		LoadCharacter(lDbManager);

		DbCharacter* lCharacter = lDbManager->GetCharacter();
		if (!lCharacter->IsLoaded()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to load character data");
			return;
		}

		LoadSettingsOverride(lDbManager, lCharacter);

		lDbManager->GetGearContext()->SetParameters(lCharacter->GetCharacterID());

		DbAbilityLoader::Load();
	}
};