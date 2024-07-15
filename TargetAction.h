#pragma once
#include "AbilityCollectionAbility.h"

class TargetAction : public AbilityCollectionAbility
{
private:
	int _TargetID = -1;
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

		PlayerClient* lSpawn = GetSpawn();

		if (!lSpawn) return false;

		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if ((*lIterator)->AbilityReady() && (*lIterator)->ConditionsMet()) {
				Utils::MikuEcho(Utils::BLUE_COLOR, "Targeted: ", lSpawn->Name);
				Utils::TargetByID(GetCharInfo()->pSpawn, lSpawn->SpawnID);
				return CastSelectedAbility(*lIterator);
			}
		}
		Reset();
		return false;
	}
};

const char* TargetAction::Key = "target";