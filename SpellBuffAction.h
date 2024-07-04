#pragma once

#include "Ability.h"
#include "AbilityCollectionAbility.h"
#include "PlayerUtils.h"
#include "PlayerUtils.h"
#include <set>

class SpellBuffAction : public AbilityCollectionAbility
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

		//PlayerClient* lSpawn = SpawnManager::Get()->GetSpawn(_SpawnType);

		//if (!lSpawn) return false;

		//Utils::MikuEcho(Utils::BLUE_COLOR, "Targeted: ", lSpawn->Name);

		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if (!(*lIterator)->ConditionsMet()) continue;

			if (!(*lIterator)->GetAbility()->Memorized()) return true;

			if (!(*lIterator)->AbilityReady()) return true;

			return CastSelectedAbility(*lIterator);
		}
		Reset();
		return false;
	}
};
const char* SpellBuffAction::Key = "onceoff";