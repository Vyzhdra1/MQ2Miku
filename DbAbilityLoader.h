#pragma once

#include <vector>
#include <mq/Plugin.h>
#include "Utils.h"
#include "DbManager.h"
#include "AbilityManager.h"
#include "SkillAbility.h"
#include "AAAbility.h"
#include "SpellAbility.h"
#include "DiscAbility.h"
#include "CommandAbility.h"
#include "ItemAbility.h"

class DbAbilityLoader
{
private:
	static void LoadSkills() {
		std::map<std::string, int> lSkills;
		lSkills["Backstab"] = 8;
		lSkills["Bash"] = 10;
		lSkills["Begging"] = 67;
		lSkills["Disarm"] = 16;
		lSkills["DragonPunch"] = 21;
		lSkills["EagleStrike"] = 23;
		lSkills["FeignDeath"] = 25;
		lSkills["FlyingKick"] = 26;
		lSkills["Forage"] = 27;
		lSkills["Frenzy"] = 74;
		lSkills["Hide"] = 29;
		lSkills["Intimidation"] = 71;
		lSkills["Kick"] = 30;
		lSkills["Mend"] = 32;
		lSkills["PickPockets"] = 48;
		lSkills["RoundKick"] = 38;
		lSkills["SenseTrap"] = 62;
		lSkills["Slam"] = 111;
		lSkills["Sneak"] = 42;
		lSkills["Taunt"] = 73;
		lSkills["TigerClaw"] = 52;

		for (const auto& [lKey, lValue] : lSkills) {
			if (SkillAbility::HasSkill(lValue)) {
				AbilityManager::Get()->AddAbility(lKey, new SkillAbility(lKey, lValue));
			}
		}
	}

	static void LoadAlternateAbilities(DbManager* aDbManager) {
		aDbManager->GetAlternateAbilitiesContext()->SetParameters(pEverQuest->GetClassThreeLetterCode(GetPcProfile()->Class));
		aDbManager->GetAlternateAbilitiesContext()->LoadObjects();
		std::map<std::string, DbAlternateAbilities*>* lAlternateAbilities = aDbManager->GetAlternateAbilitiesContext()->GetAlternateAbilities();

		for (const auto& [lKey, lValue] : *lAlternateAbilities) {
			AbilityManager::Get()->AddAbility(lKey, new AAAbility(lValue));
		}
	}

	static void LoadSpells(DbManager* aDbManager) {
		aDbManager->GetSpellsContext()->SetParameters(pEverQuest->GetClassThreeLetterCode(GetPcProfile()->Class));
		aDbManager->GetSpellsContext()->LoadObjects();
		std::map<std::string, DbSpell*>* lSpells = aDbManager->GetSpellsContext()->GetSpells();

		for (const auto& [lKey, lValue] : *lSpells) {
			if (!lValue->GetIsCombatAbility()) {
				AbilityManager::Get()->AddAbility(lKey, new SpellAbility(lValue));
			}
			else {
				AbilityManager::Get()->AddAbility(lKey, new DiscAbility(lValue));
			}
		}
	}

	static void LoadCommands(DbManager* aDbManager) {
		aDbManager->GetCommandContext()->LoadObjects();
		std::map<std::string, DbCommand*>* lCommands = aDbManager->GetCommandContext()->GetCommands();

		for (const auto& [lKey, lValue] : *lCommands) {
			AbilityManager::Get()->AddAbility(lKey, new CommandAbility(lValue));
		}
	}

	static void LoadItemAbility(DbManager* aDbManager) {
		aDbManager->GetItemAbilityContext()->SetParameters(pEverQuest->GetClassThreeLetterCode(GetPcProfile()->Class));
		aDbManager->GetItemAbilityContext()->LoadObjects();
		std::map<std::string, DbItemAbility*>* lItemAbilities = aDbManager->GetItemAbilityContext()->GetItemAbilities();

		for (const auto& [lKey, lValue] : *lItemAbilities) {
			AbilityManager::Get()->AddAbility(lKey, new ItemAbility(lValue));
		}
	}

public:
	static void Load() {
		DbManager* lDbManager = DbManager::Get();

		LoadSpells(lDbManager);
		LoadAlternateAbilities(lDbManager);
		LoadSkills();
		LoadCommands(lDbManager);
		LoadItemAbility(lDbManager);
	}
};