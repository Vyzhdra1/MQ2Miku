#pragma once
#include "Parser.h"
#include "Factory.h"
#include "ConditionFactory.h"
#include "Entry.h"
#include "Action.h"

class ConfigManager
{
public:
	void Load(MikuPlayer* aPlayer) {
		LoadAbilities(aPlayer);
		LoadSets(aPlayer);

		aPlayer->GetActionManager()->ActivatePermanentlyEnabledSets();
	}

	void PointToClassFile(MikuPlayer* aPlayer) {
		sprintf_s(INIFileName, "%s\\config\\MQ2Miku\\class\\Miku_%s.ini", gPathMQRoot, aPlayer->GetClassName().c_str());
	}

	void PointToGeneralFile() {
		sprintf_s(INIFileName, "%s\\config\\MQ2Miku\\Miku.ini", gPathMQRoot);
	}

	void LoadAbilities(MikuPlayer* aPlayer) {
		PointToGeneralFile();
		LoadAbilitiesToMap(aPlayer);

		PointToClassFile(aPlayer);
		LoadAbilitiesToMap(aPlayer);
	}

	void LoadSets(MikuPlayer* aPlayer) {
		PointToGeneralFile();
		LoadSetToMap(aPlayer);

		PointToClassFile(aPlayer);
		LoadSetToMap(aPlayer);
	}

	AbilityCollectionAbility* LoadSet(Entry* aEntry, MikuPlayer* aPlayer) {
		AbilityCollectionAbility* lCollection = Factory::BuildAbilityCollection(aEntry);

		if (!lCollection) {
			return lCollection;
		}

		LoadAbilitiesToSet(aEntry->GetID(), aPlayer, lCollection);

		if (lCollection->GetAbilityCollection().size() == 0) {
			delete lCollection;
			lCollection = 0;
		}

		return lCollection;
	}

	void LoadSetToMap(MikuPlayer* aPlayer) {
		std::vector<Entry*> lEntries = LoadSetting("Sets");

		for (std::vector<Entry*>::iterator lEntry = lEntries.begin(); lEntry != lEntries.end(); lEntry++) {

			AbilityCollectionAbility* lCollection = LoadSet(*lEntry, aPlayer);

			if (lCollection) {
				aPlayer->GetActionManager()->AddAbilitySet(lCollection->GetKey(), lCollection);
				Utils::MikuEcho(Utils::WARNING_COLOR, "Succefully Loaded Set: ", lCollection->GetKey());
			}
			else {
				Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to load Set: ", (*lEntry)->GetID());
			}


			delete* lEntry;
		}

		lEntries.clear();
	}

	void LoadAbilitiesToSet(std::string aHeader, MikuPlayer* aPlayer, AbilityCollectionAbility* aCollection) {
		std::vector<Entry*> lEntries = LoadSetting(aHeader);

		for (std::vector<Entry*>::iterator lEntry = lEntries.begin(); lEntry != lEntries.end(); lEntry++) {
			std::string lID = (*lEntry)->GetID();

			Ability* lAbility = AbilityManager::Get()->GetAbility(lID);

			Action* lAction;
			if (lAbility) {
				lAction = new Action();
				lAction->SetAbility(lAbility);
			}
			else {
				lAction = LoadSet(*lEntry, aPlayer);
			}

			if (!lAction) {
				delete* lEntry;
				Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to load Action: ", lID);
				continue;
			}

			aCollection->AddAbility(lAction);

			ConditionFactory::AddConditions(*lEntry, lAction);

			delete* lEntry;
		}

		lEntries.clear();
	}

	void LoadAbilitiesToMap(MikuPlayer* aPlayer) {
		std::vector<Entry*> lEntries = LoadSetting("Abilities");

		for (std::vector<Entry*>::iterator lEntry = lEntries.begin(); lEntry != lEntries.end(); lEntry++) {
			Ability* lAbility = Factory::BuildAbility(*lEntry, aPlayer);

			std::string lID = (*lEntry)->GetID();
			bool lLoadTrigger = (*lEntry)->GetLoadTrigger();

			delete* lEntry;

			if (!lAbility) {
				Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to load Ability to Set: ", lID);
				continue;
			}


			if (!lAbility->AbilityFound()) {
				if (lAbility->GetType().compare(SkillAbility::ConfigKey)) {
					Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to load Ability to Set: ", lID);
				}
				delete lAbility;
				continue;
			}

			AbilityManager::Get()->AddAbility(lID, lAbility);
			lAbility->EchoLoadSuccessMessage();
			if (lLoadTrigger) {
				lAbility->LoadTriggerSpells();
			}
		}
	}

	std::vector<Entry*> LoadSetting(std::string aHeader)
	{
		std::vector<Entry*> lResult;

		if (aHeader.compare("") == 0) {
			return lResult;
		}

		std::string aKeyFormat = "%i";
		DWORD lSetting = 1;
		int lParseResult = 1;
		int lIndex = 0;

		char lKey[MAX_STRING];
		char lValue[MAX_STRING];
		char lInputString[MAX_STRING];

		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loading Set: ", aHeader);
		while (lSetting)
		{
			sprintf_s(lKey, aKeyFormat.c_str(), lIndex);
			lSetting = GetPrivateProfileString(aHeader, lKey, "", (PCHAR)&lValue, 128, INIFileName);
			strcpy_s(lInputString, lValue);

			if (lSetting) {
				Entry* lEntry = new Entry();
				bool lParseSuccessful = lEntry->Parse(lValue);
				if (lParseSuccessful) {
					lResult.push_back(lEntry);
				}
				else {
					delete lEntry;
				}
			}

			lIndex++;
		}
		return lResult;
	}

	void WriteINI()
	{
		WritePrivateProfileString("Settings", "Miku", "on", INIFileName);
		WriteChatColor("\ayMikuMiku:\ax Writing INI...");
	}
};