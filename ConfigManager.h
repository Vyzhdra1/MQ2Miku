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

		ActionManager::Get()->ActivatePermanentlyEnabledSets();
	}

	void PointToClassFile(MikuPlayer* aPlayer) {
		sprintf_s(INIFileName, "%s\\config\\MQ2Miku\\class\\Miku_%s.ini", gPathMQRoot, pEverQuest->GetClassThreeLetterCode(GetPcProfile()->Class));
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

		if (!lCollection->HasAbilities() && !lCollection->GetAllowEmpty()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Set is empty: ", aEntry->GetID());
			delete lCollection;
			lCollection = 0;
		}
		else {
			aEntry->ClearType();
		}

		return lCollection;
	}

	void LoadSetToMap(MikuPlayer* aPlayer) {
		std::vector<Entry*> lEntries = LoadSetting("Sets");

		for (std::vector<Entry*>::iterator lEntry = lEntries.begin(); lEntry != lEntries.end(); lEntry++) {

 			AbilityCollectionAbility* lCollection = LoadSet(*lEntry, aPlayer);

			if (lCollection) {
				lCollection->FlagOwnedByManager();
				ActionManager::Get()->AddAbilitySet(lCollection->GetKey(), lCollection);
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
		Action* lAction = 0;
		Ability* lAbility = AbilityManager::Get()->GetAbility(aHeader);

		//Load Single Ability to Ability collection as opposed to a set of abilities. Condition will be attached to parent collection action
		if (lAbility) {
			lAction = new Action();
			lAction->SetAbility(lAbility);
			aCollection->AddAbility(lAction);
			return;
		}

		std::vector<Entry*> lEntries = LoadSetting(aHeader);
		for (std::vector<Entry*>::iterator lEntry = lEntries.begin(); lEntry != lEntries.end(); lEntry++) {
			lAction = 0;
			bool lSupressConditions = false;
			std::string lID = (*lEntry)->GetID();

			if (!(*lEntry)->GetType().empty()) {
				lAction = LoadSet(*lEntry, aPlayer);
				lSupressConditions = true;
			}
			else {
				Ability* lAbility = AbilityManager::Get()->GetAbility(lID);

				if (lAbility) {
					lAction = new Action();
					lAction->SetAbility(lAbility);
				}

				if (!lAction) {
					lAction = ActionManager::Get()->GetAbilitySet(lID);
				}

				if (!lAction) {
					lAction = LoadSet(*lEntry, aPlayer);
				}
			}

			if (!lAction) {
				delete* lEntry;
				Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to load Action: ", lID);
				continue;
			}

			aCollection->AddAbility(lAction);

			if (!lSupressConditions) {
				ConditionFactory::AddConditions(*lEntry, lAction);
			}

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
				delete lAbility;
				continue;
			}
			lAbility->EchoLoadSuccessMessage();
			if (lLoadTrigger) {
				lAbility->LoadTriggerSpells();
			}

			AbilityManager::Get()->AddAbility(lID, lAbility);
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