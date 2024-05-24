#pragma once
#include "AbilityCollectionAbility.h"

class TargetAbility : public AbilityCollectionAbility
{
public:
	static const char* Key;

	virtual bool CanCastSimultaneously() {
		return false;
	}

	bool ExecuteAbility() {
		if (_SpawnType == UNKNOWN) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not find Target for: ", GetKey());
			return false;
		}

		if (!ConditionsMet()) {
			return false;
		}

		PlayerClient* lSpawn = SpawnManager::Get()->GetSpawn(_SpawnType);

		if (!lSpawn) return false;

		Utils::MikuEcho(Utils::BLUE_COLOR, "Targeted: ", lSpawn->Name);

		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if ((*lIterator)->AbilityReady() && (*lIterator)->ConditionsMet()) {
				Utils::TargetByID(GetCharInfo()->pSpawn, lSpawn->SpawnID);
				return CastSelectedAbility(*lIterator);
			}
		}
		Reset();
		return false;
	}
};

const char* TargetAbility::Key = "target";