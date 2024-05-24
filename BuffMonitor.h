#pragma once

#include "AbilityCollectionAbility.h"
#include "PlayerUtils.h"
#include "BuffAction.h"
#include <set>

class BuffMonitor: public AbilityCollectionAbility {
private:
	unsigned long _Timer;
public:
	static const char* Key;

	bool IsBuffPollTime() {
		return _Timer > Utils::GetClockTime();
	}

	bool HasRequets() {
		return _Timer > Utils::GetClockTime();
	}

	void PollBuffs() {
		if (!IsBuffPollTime()) return;

		for (Action* lAction : _Abilities) {

		}
	}

	virtual bool CanCastSimultaneously() {
		return false;
	}

	void AddRequestors(int aRequestor, std::set<int> aSpellIDs) {

	}

	void BuffStatusResponse(int aCaller, std::string aResponse) {

	}

	void BuffStatusQuery(int aCaller, std::string aQuery) {

	}

	bool ExecuteAbility() {
		if (_Abilities.empty()) return false;

		PollBuffs();

		if (!lSpawn) return false;

		for (Action* lAction : _Abilities) {
			BuffAction* lBuffAction = (BuffAction*)lAction;

			if (!lBuffAction->ConditionsMet()) continue;

			if (!lBuffAction->HasRequesters()) continue;

			if (!lBuffAction->IsMemorized()) return true;

			if (!lBuffAction->AbilityReady()) return false;

			if ((*lIterator)->AbilityReady() && (*lIterator)->ConditionsMet()) {
				Utils::TargetByID(GetCharInfo()->pSpawn, lSpawn->SpawnID);
			}

			if (CastSelectedAbility(lBuffAction)) {

			}
		}

		Reset();
		return false;
	}

};