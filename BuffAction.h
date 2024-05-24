#pragma once

#include "Ability.h"
#include "Action.h"
#include "PlayerUtils.h"
#include <set>

class BuffAction: public Action {
private:
	SpawnType _SpawnType;
	std::set<int> _SpellIDs;
	std::set<int> _RequestIDs;

	void Memorize() {
		if (pSpellBookWnd) return;
		EzCommand("/autoinventory");
		EzCommand("/autoinventory");
		EzCommand("/autoinventory");

		int Index = 1;
		std::string lMemorizeCommand = "/memorize \"" + std::to_string(GetAbility()->GetSpellID()) + "|" + std::to_string(Index) + "\"";
		Utils::MikuSendCommand(lMemorizeCommand);
	}
public:
	SpawnType GetSpawnType() {
		return _SpawnType;
	}

	void AddRequester(int aSpawnID) {
		_RequestIDs.insert(aSpawnID);
	}

	bool HasRequesters() {
		return !_RequestIDs.empty();
	}

	std::set<int> GetIDs() {
		return _SpellIDs;
	}

	bool IsSpellMatch(int aID) {
		return _SpellIDs.find(aID) != _SpellIDs.end();
	}

	void InitSpellIDs() {
		if (!GetAbility()->GetSpellID()) return;

		if (!_Ability->HasTriggerSpells()) {
			_SpellIDs.insert(GetAbility()->GetSpellID());
		}

		std::set<PSPELL> lTriggerSpells = GetAbility()->GetTriggerSpells();

		for (PSPELL lSpell : lTriggerSpells) {
			_SpellIDs.insert(lSpell->ID);
		}
	}

	void Target() {
		if (_SpawnType == UNKNOWN) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not find Target for: ", GetKey());
			return;
		}

		//PlayerClient* lSpawn = AssessmentManager::Get()->GetGroupAssessment()->GetSpawn(_SpawnType);
	}

	bool IsMemorized() {
		if (!GetAbility()->Memorized()) {
			Memorize();
			return false;
		}
		return true;
	}
};